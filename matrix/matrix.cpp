#include "matrix.hpp"

#include "../stream_helpers.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <ctime>

//#####################################################################################################################
#define COORD(X, Y) (Y * dimension_ + X)
#define DCOORD(X, Y, dim) (Y * dim + X)
//#####################################################################################################################
Matrix::Matrix(std::string const& binaryFile, int dimension)
	: data_{}
	, dimension_{dimension}
{
	read_binary(binaryFile, dimension);
}
//---------------------------------------------------------------------------------------------------------------------
Matrix::Matrix(int dimension)
	: data_{}
	, dimension_{dimension}
{
	resize(dimension);
}
//---------------------------------------------------------------------------------------------------------------------
bool Matrix::read_binary(std::string const& filename, int dimension)
{
	std::ifstream reader{filename, std::ios_base::binary};
	if (!reader.good())
		return false;

	resize(dimension);

	for (int y = 0; y != dimension; ++y)
	{
		for (int x = 0; x != dimension; ++x)
		{
			reader.read(reinterpret_cast <char*> (&data_[y * dimension + x]), sizeof(value_type));
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

	resize(dimension);

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
			writer.write(reinterpret_cast <char*> (&data_[y * dimension_ + x]), sizeof(value_type));
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

	result.resize(dimension_);

	for (int x = 0; x != dimension_; ++x)
	{
		for (int y = 0; y != dimension_; ++y)
		{
			// TODO: Improveable
			value_type sum = 0;
			for (int i = 0; i != dimension_; ++i)
				sum += data_[COORD(i, y)] * other.data_[COORD(x, i)];
			result.data_[COORD(x, y)] = sum;
		}
	}
	return result;
}
//---------------------------------------------------------------------------------------------------------------------
Matrix Matrix::operator+(Matrix const& other)
{
	Matrix result{dimension_};
	for (int x = 0; x != dimension_; ++x)
		for (int y = 0; y != dimension_; ++y)
			result.data_[COORD(x, y)] = data_[COORD(x, y)] + other.data_[COORD(x, y)];
	return result;
}
//---------------------------------------------------------------------------------------------------------------------
int Matrix::dimension() const
{
	return dimension_;
}
//---------------------------------------------------------------------------------------------------------------------
void Matrix::resize(int dimension)
{
	dimension_ = dimension;
	data_.resize(dimension * dimension);
}
//---------------------------------------------------------------------------------------------------------------------
std::size_t Matrix::data_size() const
{
	return data_.size();
}
//---------------------------------------------------------------------------------------------------------------------
Matrix::value_type* Matrix::data()
{
	return data_.data();
}
//#####################################################################################################################
Matrix generate_random_matrix(int dimension)
{
    Matrix result{dimension};

    std::uniform_int_distribution<int> distribution(0, 9);

    std::mt19937 twister{static_cast <std::mt19937::result_type> (time(0))};
    for (int y = 0; y != dimension; ++y)
		for (int x = 0; x != dimension; ++x)
            result.data_[DCOORD(x, y, dimension)] = distribution(twister);

    return result;
}
//#####################################################################################################################
