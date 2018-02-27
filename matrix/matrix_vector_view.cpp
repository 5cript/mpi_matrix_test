#include "matrix_vector_view.hpp"
#include "matrix_vector.hpp"

#include <iostream>


//####################################################################################################################
MatrixVectorView::MatrixVectorView(MatrixVector *vector, int index)
	: vector_{vector}
	, blockDimension_{vector->get_block_dimension()}
	, index_{index}
    , offset_{index * blockDimension_ * blockDimension_}
{

}
//--------------------------------------------------------------------------------------------------------------------
int MatrixVectorView::dimension() const
{
	return blockDimension_;
}
//--------------------------------------------------------------------------------------------------------------------
Matrix::value_type* MatrixVectorView::get_line(int y) const
{
    return &vector_->data_[0] + offset_ + y * blockDimension_;
}
//--------------------------------------------------------------------------------------------------------------------
int MatrixVectorView::data_size() const
{
    return blockDimension_ * blockDimension_;
}
//--------------------------------------------------------------------------------------------------------------------
void MatrixVectorView::accum_multiply(const MatrixVectorView &with, Matrix* target)
{
    int dimension = blockDimension_;

    for (int x = 0; x != dimension; ++x)
    {
        for (int y = 0; y != dimension; ++y)
        {
            // TODO: Improveable
            int sum = 0;
            for (int i = 0; i != dimension; ++i)
            {
                sum += at(i, y) * with.at(x, i);
                //std::cout << with.at(i, y) << "\n";
            }
            target->at(x, y) += sum;
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------
void MatrixVectorView::print_matrix(int maxValues) const
{
	auto* data_ptr = get_line(0);

	for (int y = 0; y != blockDimension_; ++y)
	{
		for (int x = 0; x != blockDimension_; ++x)
		{
			std::cout << data_ptr[y * blockDimension_ + x] << ";";
			if (y * blockDimension_ + x > maxValues)
			{
				std::cout << "...\n";
				return;
			}
		}
		std::cout << "\n";
	}
}
//####################################################################################################################
