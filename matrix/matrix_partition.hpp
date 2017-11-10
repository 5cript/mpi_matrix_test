#pragma once

#include "matrix_block.hpp"

class MatrixPartition
{
public:
	/**
	 *	Partitions the matrix into blocks based on the number of instances.
	 *	@param matrix The matrix that is to be partitioned.
	 *	@param div The amount of row and column blocks. matrix->dimension must be divisible by div.
	 */
	MatrixPartition(Matrix* matrix, int div);

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
private:
	Matrix* matrix_;
	int div_;
};
