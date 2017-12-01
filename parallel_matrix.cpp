#include "parallel_matrix.hpp"

#include "matrix/matrix.hpp"
#include "matrix/matrix_partition.hpp"
#include "bench/benchhelp.hpp"
#include "mpi/communicator.hpp"
#include "parallel_matrix.hpp"

#include <iostream>

//#define DO_TRANSMIT

//####################################################################################################################
int parallel_multiplication(Mpi::Context& ctx, ProgramOptions const& options)
{
	Mpi::Communicator communicator{&ctx};

	if (!ctx.is_root())
		return 0;

    // rename
    auto const& dimension = options.dimension;

	Matrix lhs;
	Matrix rhs;
    //auto res = load_matrices(ctx, options, lhs, rhs);
    //if (res != 0)
    //    return res;

#ifdef DO_TRANSMIT
	// now spread the data from root to all other instances.
	communicator.broadcast <Matrix::value_type> (lhs.data(), lhs.data_size());
	communicator.broadcast <Matrix::value_type> (rhs.data(), rhs.data_size());
#endif // DO_TRANSMIT

	// determine the block width for the partitions (the matrix dimension is never prime)

    int REMOVE_ME = 12;

	auto blockWidthOptim = MatrixPartition::optimal_partitioning(dimension, REMOVE_ME, true);
	auto blockWidth = MatrixPartition::optimal_partitioning(dimension, REMOVE_ME);
    auto blockPerInstance = MatrixPartition::blocks_per_instance(dimension, blockWidth, REMOVE_ME);    

    std::cout << "total dimension:\t" << dimension << "\n";
    std::cout << "width of a block:\t" << blockWidth << "\n";
    std::cout << "width of block (if no idle):\t" << blockWidthOptim << "\n";
    std::cout << "block / instance:\t" << blockPerInstance << "\n";
    std::cout << "mpi instances:\t\t" << REMOVE_ME << "\n";
    std::cout << "total blocks:\t\t" << (dimension/blockWidth)*(dimension/blockWidth) << "\n";
    std::cout << "rest:\t\t\t" << (dimension/blockWidth)*(dimension/blockWidth) - blockPerInstance * REMOVE_ME << "\n";
    return 0;

    /*
	Matrix resultMatrix{dimension};
	auto resultPartition = MatrixPartition{&resultMatrix, div, ctx.size()};
	auto leftPartition = MatrixPartition{&lhs, div, ctx.size()};
	auto rightPartition = MatrixPartition{&rhs, div, ctx.size()};

    auto thisId = ctx.id();
*/

    /*
	for (int Cy = 0; Cy != div; ++Cy)
		for (int Cx = 0; Cx != div; ++Cx)
			for (int i = 0; i != div; ++i)
				leftPartition.aquire(i, Cy).accum_multiply(
					rightPartition.aquire(Cx, i), resultPartition.aquire(Cx, Cy)
				);
    */

/*
	if (ctx.is_root())
		resultMatrix.print(100);

	std::cout << "\n";

	(lhs*rhs).print();
*/
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
