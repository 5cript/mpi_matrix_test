#include "matrix_vector_view.hpp"
#include "matrix_vector.hpp"

#include <iostream>


//####################################################################################################################
MatrixVectorView::MatrixVectorView(MatrixVector *vector, int index)
	: vector_{vector}
	, index_{index}
    , offset_{index * vector->get_block_dimension() * vector->get_block_dimension()}
{
	
}
//--------------------------------------------------------------------------------------------------------------------
int MatrixVectorView::dimension() const
{
	return vector_->get_block_dimension();
}
//--------------------------------------------------------------------------------------------------------------------
Matrix::value_type* MatrixVectorView::get_line(int y) const
{
    return &vector_->data_[0] + offset_ + y * vector_->get_block_dimension();
}
//--------------------------------------------------------------------------------------------------------------------
int MatrixVectorView::data_size() const
{
    return vector_->get_block_dimension() * vector_->get_block_dimension();
}
//--------------------------------------------------------------------------------------------------------------------
void MatrixVectorView::accum_multiply(const MatrixVectorView &with, Matrix* target)
{
    int dimension = vector_->get_block_dimension();

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

	for (int y = 0; y != vector_->get_block_dimension(); ++y)
	{
		for (int x = 0; x != vector_->get_block_dimension(); ++x)
		{
			std::cout << data_ptr[y * vector_->get_block_dimension() + x] << ";";
			if (y * vector_->get_block_dimension() + x > maxValues)
			{
				std::cout << "...\n";
				return;
			}
		}
		std::cout << "\n";
	}
}
//####################################################################################################################
