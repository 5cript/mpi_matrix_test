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
			matrix_->data_[recalculateIndex(x, y)] = matrix.data_[COORD(x, y)];

	return *this;
}
//--------------------------------------------------------------------------------------------------------------------
int MatrixBlock::recalculateIndex(int localX, int localY) const
{
	return (dimension_ * x_ + localX) + (dimension_ * y_ + localY) * matrix_->dimension();
}
//--------------------------------------------------------------------------------------------------------------------
Matrix MatrixBlock::clone() const
{
	Matrix cpy{dimension_};
	for (int x = 0; x != dimension_; ++x)
	{
		for (int y = 0; y != dimension_; ++y)
		{			
			cpy.data_[COORD(x, y)] = matrix_->data_[recalculateIndex(x, y)];
		}	
	}
	return cpy;
}
//--------------------------------------------------------------------------------------------------------------------
void MatrixBlock::accum_multiply(MatrixBlock const& with, MatrixBlock target)
{	
	/*
	Matrix temp{dimension_};
	for (int x = 0; x != dimension_; ++x)
	{
		for (int y = 0; y != dimension_; ++y)
		{
			Matrix::value_type sum = 0;
			for (int i = 0; i != dimension_; ++i)
			{
				sum += matrix_->data_[COORD(i, y)] * with.matrix_->data_[COORD(x, i)];

				std::cout << "(" << i << "," << y << ")*(" << x << "," << i << ")+ ";
			}
			std::cin.get();
			std::cout << "0\n";
			temp.data_[COORD(x, y)] = sum;
		}
	}
	*/
	auto temp = with.clone() * this->clone();

	for (int x = 0; x != dimension_; ++x)
		for (int y = 0; y != dimension_; ++y)
			target.matrix_->data_[recalculateIndex(x, y)] += temp.data_[COORD(x, y)];
}
//--------------------------------------------------------------------------------------------------------------------
void MatrixBlock::print(int maxValues) const
{
	for (int y = 0; y != dimension_; ++y)
	{
		for (int x = 0; x != dimension_; ++x)
		{
			std::cout << matrix_->data_[recalculateIndex(x, y)] << ";";	
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
