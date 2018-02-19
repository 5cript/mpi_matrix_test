#include "matrix_vector.hpp"

#include <iostream>


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
std::size_t MatrixVector::data_size() const
{
    return data_.size();
}
//--------------------------------------------------------------------------------------------------------------------
int MatrixVector::get_block_dimension() const
{
	return blockDimension_;
}
//--------------------------------------------------------------------------------------------------------------------
void MatrixVector::multiply_with(MatrixVector& other, Matrix* target)
{
    target->clear();

    //get(1).accum_multiply(other.get(1), target);
    for (int i = 0; i != size(); ++i)
    {
        get(i).accum_multiply(other.get(i), target);
    }
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
