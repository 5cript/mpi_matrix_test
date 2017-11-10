#pragma once

#include "forward.hpp"
#include "matrix.hpp"

/**
 *	A block within a matrix partition.
 */
class MatrixBlock
{
public:
	MatrixBlock(Matrix* matrix, int x, int y, int dimension);

	/**
	 *	target += this x with.
	 */
	void accum_multiply(MatrixBlock* with, MatrixBlock* target);

private:
	Matrix* matrix_;
	int x_;
	int y_;
	int dimension_;
};
