#pragma once

#include "forward.hpp"
#include "matrix.hpp"
#include "matrix_vector_view.hpp"

#include <vector>

/**
 *	A vector of matrices;
 */
class MatrixVector
{
	friend MatrixVectorView;

public:
	MatrixVector(int blockDimension, int vectorSize);

	MatrixVector& operator=(MatrixVector const&) = delete;
	MatrixVector& operator=(MatrixVector&&) = delete;
	MatrixVector(MatrixVector const&) = delete;
	MatrixVector(MatrixVector&&) = delete;

	int get_block_dimension();
	int size();
	MatrixVectorView get(int index);
	Matrix::value_type* data();

private:
	std::vector <Matrix::value_type> data_;
	int blockDimension_;
	int vectorSize_;
};
