#pragma once

#include "forward.hpp"
#include "matrix.hpp"

/**
 *	A matrix vector view is a matrix like view over a vector.
 */
class MatrixVectorView
{
public:
	MatrixVectorView(MatrixVector* vector, int index);

	MatrixVectorView(MatrixVectorView const&) = default;
	MatrixVectorView(MatrixVectorView&&) = default;
	MatrixVectorView& operator=(MatrixVectorView const&) = default;
	MatrixVectorView& operator=(MatrixVectorView&&) = default;

	~MatrixVectorView() = default;
	
	/**
	 *	Retrieve as line.
	 */
	Matrix::value_type* get_line(int y) const;

	/**
	 *	Retrieve Matrix Block dimension.
	 */
	int dimension() const;

	/**
	 *	Print Matrix.
	 */
	void print_matrix(int maxValues = 100) const;

private:
	MatrixVector* vector_;
	int index_;
	int offset_;
};
