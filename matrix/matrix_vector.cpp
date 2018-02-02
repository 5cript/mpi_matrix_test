#include "matrix_vector.hpp"

//####################################################################################################################
MatrixVector::MatrixVector(int blockDimension, int vectorSize)
	: data_(blockDimension*blockDimension*vectorSize) // intentional std::vector() constructor
	, blockDimension_{blockDimension}
	, vectorSize_{vectorSize}
{
	
}
//--------------------------------------------------------------------------------------------------------------------
Matrix::value_type* MatrixVector::data()
{
	return &data_.front();
}	
//--------------------------------------------------------------------------------------------------------------------
int MatrixVector::get_block_dimension()
{
	return blockDimension_;
}
//--------------------------------------------------------------------------------------------------------------------
int MatrixVector::size()
{
	return vectorSize_;
}
//--------------------------------------------------------------------------------------------------------------------
MatrixVectorView MatrixVector::get(int index)
{
	return {this, index};
}
//####################################################################################################################
