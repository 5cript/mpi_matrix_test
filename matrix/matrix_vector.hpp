#pragma once

#include "matrix.hpp"

#include <vector>

/**
 *	A vector of matrices;
 */
class MatrixVector
{
public:
	MatrixVector(int size);
	MatrixVector();

	MatrixVector& operator=(MatrixVector const&) = delete;
	MatrixVector& operator=(MatrixVector&&) = delete;
	MatrixVector(MatrixVector const&) = delete;
	MatrixVector(MatrixVector&&) = delete;

	std::vector <Matrix>* get();
	Matrix* get(std::size_t which);
private:
	std::vector <Matrix> matrices_;
};
