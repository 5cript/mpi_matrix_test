#include "matrix_block.hpp"
#include "matrix_partition.hpp"

#include <iostream>
#include <stdexcept>

#define COORD(X, Y) (Y * dimension_ + X)

//####################################################################################################################
MatrixBlock::MatrixBlock(Matrix* matrix, int x, int y, int dimension)
	: matrix_{matrix}
	, x_{x}
	, y_{y}
	, dimension_{dimension}
{

}
//--------------------------------------------------------------------------------------------------------------------
MatrixBlock& MatrixBlock::operator=(Matrix const& matrix)
{
	if (matrix.dimension() != dimension_)
		throw std::out_of_range("matrix dimensions are not equal");

	for (int x = 0; x != dimension_; ++x)
		for (int y = 0; y != dimension_; ++y)
			at(x, y) = matrix.data_[COORD(x, y)];

	return *this;
}
//--------------------------------------------------------------------------------------------------------------------
Matrix MatrixBlock::clone() const
{
	Matrix cpy{dimension_};
	for (int x = 0; x != dimension_; ++x)
	{
		for (int y = 0; y != dimension_; ++y)
		{
			cpy.data_[COORD(x, y)] = at(x, y);
		}
	}
	return cpy;
}
//--------------------------------------------------------------------------------------------------------------------
void MatrixBlock::clear()
{
	for (int x = 0; x != dimension_; ++x)
		for (int y = 0; y != dimension_; ++y)
			at(x, y) = 0;
}
//--------------------------------------------------------------------------------------------------------------------
void MatrixBlock::accum_multiply(MatrixBlock const& with, MatrixBlock target)
{
	for (int x = 0; x != dimension_; ++x)
	{
		for (int y = 0; y != dimension_; ++y)
		{
			// TODO: Improveable
			int sum = 0;
			for (int i = 0; i != dimension_; ++i)
				sum += at(i, y) * with.at(x, i);
			target.at(x, y) += sum;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
void MatrixBlock::print(int maxValues) const
{
	for (int y = 0; y != dimension_; ++y)
	{
		for (int x = 0; x != dimension_; ++x)
		{
			std::cout << at(x, y) << ";";
			if (y * dimension_ + x > maxValues)
			{
				std::cout << "...\n";
				return;
			}
		}
		std::cout << "\n";
	}
}
//####################################################################################################################
