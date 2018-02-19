#include "matrix_partition.hpp"

#include <stdexcept>
#include <cmath>
#include <iostream>

//####################################################################################################################
MatrixPartition::MatrixPartition(Matrix* matrix, int div, int)
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
//--------------------------------------------------------------------------------------------------------------------
int MatrixPartition::optimal_partitioning(int dimension, int mpiInstances, bool preventIdle)
{
    if (mpiInstances < 1)
        throw std::invalid_argument("mpiInstances needs to be at least 1");

    auto wmax = static_cast <double> (dimension) / std::sqrt(static_cast <double> (mpiInstances));

    auto w = static_cast <int> (std::floor(wmax));
    for (; w > 0; --w)
    {
        auto minCondition = (dimension%w == 0); // && (dimension/w) % 2 == 0 && w % 2 == 0
        if (!preventIdle && minCondition)
            return w;            
        if (preventIdle && minCondition && ((dimension/w)*(dimension/w))%mpiInstances == 0)
            return w;
    }
    return w;
}
//--------------------------------------------------------------------------------------------------------------------
std::set <int> MatrixPartition::optimal_instance_counts(int dimension)
{
    std::set <int> res{};
    for (int m = 1; m != dimension; ++m)
    {
        int w = MatrixPartition::optimal_partitioning(dimension, m, true);
        if (w != 0)
        {
            res.insert(m);
        }
    }
    return res;
}
//####################################################################################################################
