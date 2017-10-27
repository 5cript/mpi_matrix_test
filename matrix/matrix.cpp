#include "matrix.hpp"

#include <fstream>

//#####################################################################################################################
void matrix::read_binary(std::string const& filename, int dimension)
{
	data_.resize(dimension * dimension);	
}
//---------------------------------------------------------------------------------------------------------------------
void matrix::read_data(std::string const& filename, int dimension)
{
	std::ifstream reader{filename, std::ios_base::binary};
	data_.resize(dimension * dimension);

	int row = 0;
	int col = 0;
	for (; col != dimension; ++col)
	{
		for (; row != dimension; ++row)
		{
			reader >> data_[col * row] >> ';';
		}
		reader >> '\n';
	}
}
//#####################################################################################################################
