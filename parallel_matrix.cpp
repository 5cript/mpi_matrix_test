#include "parallel_matrix.hpp"

#include "bench/benchhelp.hpp"
#include "container_print.hpp"
#include "matrix_loader.hpp"

#include "matrix/matrix.hpp"
#include "matrix/matrix_partition.hpp"
#include "matrix/block_list.hpp"

#include "mpi/communicator.hpp"
#include "mpi/group.hpp"
#include "mpi/shared_file.hpp"

#include <iostream>
#include <memory>
#include <algorithm>    

#define DO_TRANSMIT

//####################################################################################################################
int parallel_multiplication(Mpi::Context& ctx, ProgramOptions const& options)
{

	TimeStampCollection stamps;
	Mpi::Communicator communicator{&ctx};
	stamps.add("communicator_constructed");

    // rename
    auto const& dimension = options.dimension;

	Matrix lhs;
	Matrix rhs;
    auto res = load_matrices(ctx, options, lhs, rhs);
    if (res != 0)
        return res;

#ifdef DO_TRANSMIT
	// now spread the data from root to all other instances.
	bool legacy_broadcast = false;	
	if (legacy_broadcast)
	{	
		stamps.add("broadcast_whole");
		communicator.broadcast <Matrix::value_type> (lhs.data(), lhs.data_size());
		communicator.broadcast <Matrix::value_type> (rhs.data(), rhs.data_size());
		stamps.add("broadcast_whole_end");
	}
#endif // DO_TRANSMIT

	// determine the block width for the partitions (the matrix dimension is never prime)
	auto instanceCount = ctx.size();

	int blockWidth = MatrixPartition::optimal_partitioning(dimension, instanceCount);
	int div = dimension/blockWidth;

	if (ctx.is_root())
	{
		std::cout << "total dimension:\t" << dimension << "\n";
		std::cout << "width of a block:\t" << blockWidth << "\n";
		std::cout << "mpi instances:\t\t" << instanceCount << "\n";
		std::cout << "total blocks:\t\t" << (dimension/blockWidth)*(dimension/blockWidth) << "\n";
		std::cout << "rest:\t\t\t" << (dimension/blockWidth)*(dimension/blockWidth) - instanceCount << "\n";
	}

	
	/// REMOVE ME
	if (!ctx.is_root())
		return 0;

	PullInstruction pulledBlocks{ctx.id(), ctx.size(), div};
	std::cout << pulledBlocks.left_ << "\n";
	std::cout << pulledBlocks.right_ << "\n";
	return 0;
	/// REMOVE ME END

	// result can be just one block.
	Matrix resultBlock{blockWidth};	
	auto resultBlockView = MatrixPartition{&resultBlock, 1, ctx.size()};

	auto leftPartition = MatrixPartition{&lhs, div, ctx.size()};
	auto rightPartition = MatrixPartition{&rhs, div, ctx.size()};

	auto multiplyStep = [&](int offset)
	{
		auto pos = getMyCoordinates(ctx, div, offset);
		auto& x = pos.first;
		auto& y = pos.second;

		for (int i = 0; i != div; ++i)
		{
			leftPartition.aquire(i, y).accum_multiply(
				rightPartition.aquire(x, i), resultBlockView.aquire(0, 0) // rP(x, y)
			);
		}
		//std::cout << "*(" << x << "," << y << ")\n";
	};

	// root needs the entire matrix for gather operations.
	std::unique_ptr <std::vector <Matrix::value_type>> overallResult;

	// Step1: Do the spread multiplication (excl those blocks which do not fit if not optimal)
	resultBlock.clear();
	multiplyStep(0);

	// Save individual parts on hard drive.
	if (options.saveChunks)
		save_intermediary(resultBlock, ctx, options, 1);

	// Gather over network.
	if (options.writeStrategy == WriteStrategy::Gather)
	{
		overallResult = std::make_unique <decltype(overallResult)::element_type>();

		if (ctx.is_root())
			overallResult->resize(dimension*dimension);

		stamps.add("gather_1_start");
		communicator.gather(resultBlock.data(), ctx.is_root() ? overallResult->data() : nullptr, resultBlock.data_size());
		stamps.add("gather_1_end");
	}

	std::unique_ptr <Mpi::SharedMatrixFile> sharedFile;
	// 4.2 / 4.3
	if (options.writeStrategy == WriteStrategy::SharedFile)
	{
		sharedFile = std::make_unique <Mpi::SharedMatrixFile>(
			MPI_COMM_WORLD, 
			options.resultMatrix, 
			Mpi::FileStreamDirection::Write
		);	

		stamps.add("write_1_start");
		auto pos = getMyCoordinates(ctx, div, 0);
		sharedFile->writeBlock({&resultBlock, pos.first, pos.second}, dimension);
		stamps.add("write_1_end");
	}

	// Step2: Do the remaining multiplications.
	if (div*div > ctx.size() && ctx.id() < div*div - ctx.size())
	{
		resultBlock.clear();
		multiplyStep(ctx.size());

		// 3.5
		if (options.saveChunks)
			save_intermediary(resultBlock, ctx, options, 2);

		// 5 
		if (options.writeStrategy == WriteStrategy::Gather)
		{		
			int incrementor = 0;
			std::vector <int> subGroupIds(div*div - ctx.size());
			std::generate (subGroupIds.begin(), subGroupIds.end(), [&incrementor](){return incrementor++;});
			//std::cout << "(" << subGroupIds.size() << ")" << subGroupIds << "\n";

			Mpi::WorldGroup world;
			Mpi::SubGroup remaining{world, subGroupIds};
			Mpi::Communicator subCom{&ctx, remaining.create_communicator()};

			stamps.add("gather_2_start");
			subCom.gather(
				resultBlock.data(), 
				ctx.is_root() ? (overallResult->data() + ctx.size() * (blockWidth * blockWidth)) : nullptr,
				resultBlock.data_size()
			);
			stamps.add("gather_2_end");
		}
		// 4.2 / 4.3
		else if (options.writeStrategy == WriteStrategy::SharedFile)
		{
			auto pos = getMyCoordinates(ctx, div, ctx.size());
			stamps.add("write_2_start");
			sharedFile->writeBlock({&resultBlock, pos.first, pos.second}, dimension);
			stamps.add("write_2_end");
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
		
	// 5
	if (options.writeStrategy == WriteStrategy::Gather)
	{
		stamps.add("saving_at_root");
		if (ctx.is_root())
		{
			// Overall Result contains a sequence of blocks
			BlockList blocks{overallResult.get(), blockWidth, dimension};
			//blocks.save_block_sequence("blocky.txt");
			blocks.save_matrix(options.resultMatrix, options.humanReadableOutput);
		}
		stamps.add("saving_at_root_end");
	}

	if (ctx.is_root())
	{
		Matrix conv{"/bigwork/nhmqebbe/data/M_100x100.bin", 100};
		conv.write_data("/bigwork/nhmqebbe/data/M_100x100.data");
	}

	stamps.add("completion");
	if (ctx.is_root())
		stamps.dump();
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------
int load_matrices(Mpi::Context& ctx, ProgramOptions const& options, Matrix& lhs, Matrix& rhs)
{
    if (ctx.is_root())
	{
	    //bool(Matrix::*reader)(std::string const&, int) = &Matrix::read_binary;
        auto reader = &Matrix::read_binary;
		
		if (options.humanReadableInput)
		    reader = &Matrix::read_data;

		// root must load the matrix from disk.
		if(!(lhs.*reader)(options.leftMatrix, options.dimension))
		{
			std::cerr << "matrix does not exist: " << options.leftMatrix << "\n";
			return 2;
		}
		if(!(rhs.*reader)(options.rightMatrix, options.dimension))
		{
			std::cerr << "matrix does not exist: " << options.rightMatrix << "\n";
			return 2;
		}
	}
	else
	{
		// those who are not root shall reserve space for this.
		lhs.resize(options.dimension);
		rhs.resize(options.dimension);
	}
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------
std::set <int> optimal_instances(int dimension)
{
    return MatrixPartition::optimal_instance_counts(dimension);
}
//--------------------------------------------------------------------------------------------------------------------
std::pair <int, int> getMyCoordinates(Mpi::Context& ctx, int blocksPerRow, int offset)
{
	auto offsetId = ctx.id() + offset;
	if (offsetId >= blocksPerRow*blocksPerRow)
		throw std::runtime_error("This is not possible??");

	// where am i? What block am i?
	auto y = offsetId / blocksPerRow;
	auto x = offsetId % blocksPerRow;

	return {x, y};
}
//--------------------------------------------------------------------------------------------------------------------
void save_intermediary(Matrix const& matrix, Mpi::Context& ctx, ProgramOptions const& options, int suffix)
{
	if (!options.chunksAreHumanReadable)
		matrix.write_binary(options.partResultDirectory + "/result." + std::to_string(ctx.id()) + "_" + std::to_string(suffix));
	else
		matrix.write_data(options.partResultDirectory + "/result." + std::to_string(ctx.id()) + "_" + std::to_string(suffix));
}
//####################################################################################################################
