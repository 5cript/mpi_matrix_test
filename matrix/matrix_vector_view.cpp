#include "matrix_vector_view.hpp"
#include "matrix_vector.hpp"

#include <iostream>


//####################################################################################################################
MatrixVectorView::MatrixVectorView(MatrixVector* vector, int index)
	: vector_{vector}
	, index_{index}
	, offset_{index * vector->get_block_dimension()}
{
	
}
//--------------------------------------------------------------------------------------------------------------------
Matrix::value_type* MatrixVectorView::get_line(int y) const
{
	return &vector_->data_.front() + offset_ + y * vector_->get_block_dimension();
}
//--------------------------------------------------------------------------------------------------------------------
int MatrixVectorView::dimension() const
{
	return vector_->get_block_dimension();
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
