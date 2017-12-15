#pragma once

#include "forward.hpp"
#include "matrix_block.hpp"

#include <set>

class MatrixPartition
{
public:
	/**
	 *	Partitions the matrix into blocks based on the number of instances.
	 *	@param matrix The matrix that is to be partitioned.
	 *	@param div The amount of row and column blocks. matrix->dimension must be divisible by div.
	 */
	MatrixPartition(Matrix* matrix, int div, int mpiInstances);

	/**
	 *	Get the matrix partition width.
	 */
	int width();

	/**
	 *	Get the matrix partition height.
	 */
	int height();

	/**
	 *	Get the matrix block at the specified location.
	 */
	MatrixBlock aquire(int x, int y);

    /**
     *  Determine a number of row and column blocks that are optimal for the 
     *  amount of mpi instances and the matrix size.
     *
     *  @return Returns the dimension of a block in the partitioned matrix.
     */
    static int optimal_partitioning(int dimension, int mpiInstances, bool preventIdle = false);

    /**
     *  Tries to optimize the amount of mpiInstances so that none is idle and work is evenly distributed.
     */
    static std::set <int> optimal_instance_counts(int dimension);

private:
	Matrix* matrix_;
	int div_;
};
