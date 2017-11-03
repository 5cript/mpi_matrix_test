#include "matrix.hpp"

#include "../stream_helpers.hpp"
#include <fstream>
#include <iostream>

//#####################################################################################################################
#define COORD(X, Y) (Y * dimension_ + X)
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
	if (!writer.good())
		return false;

	for (int y = 0; y != dimension_; ++y)
	{
		for (int x = 0; x != dimension_; ++x)
		{	
			writer.write(reinterpret_cast <char*> (&data_[y * dimension_ + x]), sizeof(int_type));
		}
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------------------
bool Matrix::write_data(std::string const& filename)
{
	std::ofstream writer{filename, std::ios_base::binary};
	if (!writer.good())
		return false;

	for (int y = 0; y != dimension_; ++y)
	{
		for (int x = 0; x != dimension_; ++x)
		{	
			writer << data_[y * dimension_ + x];
			writer << ";";
		}
		writer.put('\n');
	}
	return true;
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
				return;
			}
		}
		std::cout << "\n";
	}
}
//---------------------------------------------------------------------------------------------------------------------
Matrix Matrix::operator*(Matrix const& other)
{
	Matrix result;
	
	result.dimension_ = dimension_;
	result.data_.resize(dimension_ * dimension_);

	for (int x = 0; x != dimension_; ++x)
	{
		for (int y = 0; y != dimension_; ++y)
		{
			int_type sum = 0;
			for (int i = 0; i != dimension_; ++i)
				sum += data_[COORD(i, y)] * other.data_[COORD(x, i)];
			result.data_[COORD(x, y)] = sum;
		}
	}
	return result;
}
//#####################################################################################################################
