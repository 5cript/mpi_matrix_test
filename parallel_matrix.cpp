#include "parallel_matrix.hpp"

#include "bench/benchhelp.hpp"
#include "container_print.hpp"

#include "matrix/matrix.hpp"
#include "matrix/matrix_partition.hpp"

#include "mpi/communicator.hpp"
#include "mpi/group.hpp"

#include <iostream>
#include <memory>
#include <algorithm>    

#define DO_TRANSMIT

//####################################################################################################################
int parallel_multiplication(Mpi::Context& ctx, ProgramOptions const& options)
{
	Mpi::Communicator communicator{&ctx};

    // rename
    auto const& dimension = options.dimension;

	Matrix lhs;
	Matrix rhs;
    auto res = load_matrices(ctx, options, lhs, rhs);
    if (res != 0)
        return res;

#ifdef DO_TRANSMIT
	// now spread the data from root to all other instances.
	communicator.broadcast <Matrix::value_type> (lhs.data(), lhs.data_size());
	communicator.broadcast <Matrix::value_type> (rhs.data(), rhs.data_size());
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

	// result can be just one block.
	Matrix resultBlock{blockWidth};	
	auto resultBlockView = MatrixPartition{&resultBlock, 1, ctx.size()};

	auto leftPartition = MatrixPartition{&lhs, div, ctx.size()};
	auto rightPartition = MatrixPartition{&rhs, div, ctx.size()};

	auto multiplyStep = [&](int offset)
	{
		auto offsetId = ctx.id() + offset;
		if (offsetId >= div*div)
			return;

		auto y = offsetId / div;
		auto x = offsetId / div;
		for (int i = 0; i != div; ++i)
		{
			leftPartition.aquire(i, y).accum_multiply(
				rightPartition.aquire(x, i), resultBlockView.aquire(0, 0) // rP(x, y)
			);
		}
	};


	// root needs the entire matrix.
	std::vector <Matrix::value_type> overallResult;
	if (ctx.is_root())
		overallResult.resize(dimension*dimension);

	// Step1: Do the spread multiplication (excl those blocks which do not fit if not optimal)
	resultBlock.clear();
	multiplyStep(0);

	// Gather data from first multiplication step
	communicator.gather(resultBlock.data(), ctx.is_root() ? overallResult.data() : nullptr, resultBlock.data_size());

	// Step2: Do the remaining multiplications.
	if (div*div > ctx.size())
	{
		multiplyStep(ctx.size());
		// create group and gather
		// TODO...	

		int incrementor = 0;
		std::vector <int> subGroupIds(div*div - ctx.size());
		std::generate (subGroupIds.begin(), subGroupIds.end(), [&incrementor](){return incrementor++;});
		//std::cout << "(" << subGroupIds.size() << ")" << subGroupIds << "\n";

		Mpi::WorldGroup world;
		Mpi::SubGroup remaining{world, subGroupIds};
		Mpi::Communicator subCom{&ctx, remaining.create_communicator()};
		subCom.gather(
			resultBlock.data(), 
			ctx.is_root() ? (overallResult.data() + ctx.size() * blockWidth) : nullptr,
			resultBlock.data_size()
		);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	
	if (ctx.is_root())
	{
		// OVERALL RESULT NOW CONTAINS A PASTED BLOCK 
	}

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
//####################################################################################################################
