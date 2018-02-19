#include "parallel_matrix.hpp"

#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>

#define DO_TRANSMIT

//####################################################################################################################
ParallelContext::ParallelContext(Mpi::Context& ctx, ProgramOptions const& options)
	: ctx{ctx}
	, options{options}
	, communicator{&ctx}
	, stamps{}
	, dimension{options.dimension}
	, blockWidth{0}
	, div{0}
    , x{0}
    , y{0}
    , writeAtAll{false}
	, leftSharedFile_{MPI_COMM_WORLD, options.leftMatrix, Mpi::FileStreamDirection::Read}
	, rightSharedFile_{MPI_COMM_WORLD, options.rightMatrix, Mpi::FileStreamDirection::Read}
	, resultSharedFile_{MPI_COMM_WORLD, options.resultMatrix, Mpi::FileStreamDirection::Write}
    , storage_{}
{
    stamps.add("Stamp Maker Constructed");
}
//--------------------------------------------------------------------------------------------------------------------
void ParallelContext::legacy_broadcast()
{		
	/*	
	stamps.add("broadcast_whole");
	communicator.broadcast <Matrix::value_type> (lhs.data(), lhs.data_size());
	communicator.broadcast <Matrix::value_type> (rhs.data(), rhs.data_size());
	stamps.add("broadcast_whole_end");
	*/
}
//--------------------------------------------------------------------------------------------------------------------
void ParallelContext::calculate_partitioning()
{
	blockWidth = MatrixPartition::optimal_partitioning(dimension, ctx.size());
    div = dimension/blockWidth;
    x = ctx.id() % div;
    y = ctx.id() / div;
}
//--------------------------------------------------------------------------------------------------------------------
void ParallelContext::use_write_at_all(bool waa)
{
    writeAtAll = waa;
}
//--------------------------------------------------------------------------------------------------------------------
void ParallelContext::dump_stamps_sync()
{
    if (ctx.is_root())
        stamps.dump();
}
//--------------------------------------------------------------------------------------------------------------------
void ParallelContext::imbue_views()
{
    auto view = Mpi::make_matrix_file_view(blockWidth);
    resultSharedFile_.imbue_view(view);

    if (!options.humanReadableInput)
    {
        leftSharedFile_.imbue_view(view);
        rightSharedFile_.imbue_view(view);
    }
}
//--------------------------------------------------------------------------------------------------------------------
void ParallelContext::show_partitioning()
{
	if (ctx.is_root())
	{
		std::cout << "total dimension:\t" << dimension << "\n";
		std::cout << "width of a block:\t" << blockWidth << "\n";
		std::cout << "mpi instances:\t\t" << ctx.size() << "\n";
		std::cout << "total blocks:\t\t" << (dimension/blockWidth)*(dimension/blockWidth) << "\n";
		std::cout << "rest:\t\t\t" << (dimension/blockWidth)*(dimension/blockWidth) - ctx.size() << "\n";
	}
}
//--------------------------------------------------------------------------------------------------------------------
void ParallelContext::do_multiplication()
{
    stamps.add("Begin Multiplication");
    Matrix result;
    result.resize(blockWidth);
    storage_->left()->multiply_with(*storage_->right(), &result);
    stamps.add("End Multiplication");

    if (options.saveChunks)
    {
        stamps.add("Start Intermediary Save");
        save_intermediary(result, ctx, options, ctx.id());
        stamps.add("End Intermediary Save");
    }
    else
    {
        stamps.add("Start Write Block");
        resultSharedFile_.writeBlock(BlockDescriptor <Matrix> {&result, x, y}, dimension, writeAtAll);
        stamps.add("End Write Block");
    }
}
//--------------------------------------------------------------------------------------------------------------------
void ParallelContext::perform()
{
	// determine the block width for the partitions (the matrix dimension is never prime)
	calculate_partitioning();
	show_partitioning();

    if ((dimension/blockWidth)*(dimension/blockWidth) - ctx.size() != 0)
    {
        throw std::runtime_error{"Later in development, it has been decided, that imperfect partitionings (divisibles) are disallowed"};
    }

	load_blocks();
    do_multiplication();

    MPI_Barrier(MPI_COMM_WORLD);

    stamps.add("Done");
}
//--------------------------------------------------------------------------------------------------------------------
void ParallelContext::load_blocks(int cycle)
{
    storage_ = std::make_unique <MatrixStorage> (&communicator, cycle * ctx.size() + ctx.id(), ctx.size(), div, blockWidth);

    stamps.add("Load Local Start");
    storage_->load_local(leftSharedFile_, rightSharedFile_);
    stamps.add("Load Local End");
    stamps.add("Share Blocks Start");
    storage_->share_blocks();
    stamps.add("Share Blocks Local End");
}
//####################################################################################################################
int parallel_multiplication(Mpi::Context&, ProgramOptions const&)
{
	return 255;
	/*
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
	*/
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
