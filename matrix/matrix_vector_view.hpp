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

    int data_size() const;

    /**
     * @brief Multiply this * with and add it to target.
     * @param other Another matrix within the vector.
     * @param target The matrix to add the result to.
     */
    void accum_multiply(MatrixVectorView const& with, Matrix* target);

    inline Matrix::value_type& at(int x, int y) const
    {
        return *(get_line(y) + x);
    }

	/**
	 *	Print Matrix.
	 */
	void print_matrix(int maxValues = 100) const;

private:
    MatrixVector* vector_;
    int blockDimension_;
	int index_;
	int offset_;
};
