#include "matrix.hpp"

#include "../stream_helpers.hpp"
#include <fstream>
#include <iostream>

//#####################################################################################################################
bool Matrix::read_binary(std::string const& filename, int dimension)
{
	std::ifstream reader{filename, std::ios_base::binary};
	if (!reader.good())
		return false;

	dimension_ = dimension;
	data_.resize(dimension * dimension);	

	for (int y = 0; y != dimension; ++y)
	{
		for (int x = 0; x != dimension; ++x)
		{	
			reader.read(reinterpret_cast <char*> (&data_[y * dimension + x]), sizeof(int_type));
		}
	}

	return true;
}
//---------------------------------------------------------------------------------------------------------------------
bool Matrix::read_data(std::string const& filename, int dimension)
{
	dimension_ = dimension;
	std::ifstream reader{filename, std::ios_base::binary};
	if (!reader.good())
		return false;

	data_.resize(dimension * dimension);

	for (int y = 0; y != dimension; ++y)
	{
		for (int x = 0; x != dimension; ++x)
		{	
			reader >> data_[y * dimension + x] >> ';';
		}
		reader.seekg(1, std::ios_base::cur);
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------------------
bool Matrix::write_binary(std::string const& filename)
{
	std::ofstream writer{filename, std::ios_base::binary};
	for (int y = 0; y != dimension_; ++y)
	{
		for (int x = 0; x != dimension_; ++x)
		{	
			writer.write(reinterpret_cast <char*> (&data_[y * dimension_ + x]), sizeof(int_type));
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------
void Matrix::print(int maxValues)
{
	for (int y = 0; y != dimension_; ++y)
	{
		for (int x = 0; x != dimension_; ++x)
		{
			std::cout << data_[y * dimension_ + x] << ";";	
			if (y * dimension_ + x > maxValues)
			{
				std::cout << "...\n";
				break;
			}
		}
		std::cout << "\n";
	}
}
//---------------------------------------------------------------------------------------------------------------------
Matrix Matrix::operator*(Matrix const& other);
{

}
//#####################################################################################################################
