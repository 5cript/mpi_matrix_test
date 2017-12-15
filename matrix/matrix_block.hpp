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
 	 *	sets all matrix entries to 0.
	 */
	void clear();

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
	 *  Returns value at position x, y in the view.
	 */
	Matrix::value_type& at(int x, int y)
	{
	    return matrix_->data_[
            dimension_ * matrix_->dimension() * y_ +
            matrix_->dimension() * y +
            dimension_ * x_ +
            x
        ];
	}

	/**
	 *  Returns value at position x, y in the view.
	 */
	Matrix::value_type at(int x, int y) const
	{
	    return matrix_->data_[
            dimension_ * matrix_->dimension() * y_ +
            matrix_->dimension() * y +
            dimension_ * x_ +
            x
        ];
	}

private:
	Matrix* matrix_;
	int x_;
	int y_;
	int dimension_;
};
