#include "matrix_partition.hpp"

//####################################################################################################################
MatrixPartition::MatrixPartition(Matrix* matrix, int div)
	: matrix_{matrix}
	, div_{div}
{
	
}
//--------------------------------------------------------------------------------------------------------------------
int MatrixPartition::width()
{
	return div_;
}
//--------------------------------------------------------------------------------------------------------------------
int MatrixPartition::height()
{
	return div_;
}
//--------------------------------------------------------------------------------------------------------------------
MatrixBlock MatrixPartition::aquire(int x, int y)
{
	return MatrixBlock{matrix_, x, y, matrix_->dimension() / div_};
}
//####################################################################################################################