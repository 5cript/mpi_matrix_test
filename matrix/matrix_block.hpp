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
 	 *	Copies a matrix to the view place, use with care.
	 */
	MatrixBlock& operator=(Matrix const& matrix);

	/**
	 *	target += this x with.
	 */
	void accum_multiply(MatrixBlock const& with, MatrixBlock target);

	/** 
	 *	For debugging purposes only.
	 */
	void print(int maxValues = 100) const;

	/**
 	 *	Creates an actual new matrix from this one. (costly)
	 */
	Matrix clone() const;

	/**
	 *	Recalculates a local index to the parent matrix index.
	 */
	int recalculateIndex(int localX, int localY) const;

private:
	Matrix* matrix_;
	int x_;
	int y_;
	int dimension_;
};
