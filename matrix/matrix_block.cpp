#include "matrix_block.hpp"
#include "matrix_partition.hpp"


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
void MatrixBlock::accum_multiply(MatrixBlock const& with, MatrixBlock target)
{	
	Matrix temp{dimension_};
	for (int x = 0; x != dimension_; ++x)
	{
		for (int y = 0; y != dimension_; ++y)
		{
			Matrix::value_type sum = 0;
			for (int i = 0; i != dimension_; ++i)
				sum += matrix_->data_[COORD(i, y)] * with.matrix_->data_[COORD(x, i)];
			temp.data_[COORD(x, y)] = sum;
		}
	}

	for (int x = 0; x != dimension_; ++x)
		for (int y = 0; y != dimension_; ++y)
			target.matrix_->data_[COORD(x, y)] += temp.data_[COORD(x, y)];
}
//####################################################################################################################
