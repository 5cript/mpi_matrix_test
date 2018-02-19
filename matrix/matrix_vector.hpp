#pragma once

#include "forward.hpp"
#include "matrix.hpp"
#include "matrix_vector_view.hpp"
#include "matrix_block.hpp"

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

    /**
     * @brief Returns the dimension of a block.
     */
    int get_block_dimension() const;

    /**
     * @brief Returns the size of the vector.
     */
	int size();

    /**
     * @brief Returns the size of the data block.
     * @return
     */
    std::size_t data_size() const;

    /**
     * @brief Get a matrix view from the vector.
     * @param index Which index.
     * @return A view that can be used like a matrix, but operates on this vectors data.
     */
    MatrixVectorView get(int index);
	Matrix::value_type* data();

    void multiply_with(MatrixVector& other, Matrix* target);

private:
	std::vector <Matrix::value_type> data_;
	int blockDimension_;
	int vectorSize_;
};
