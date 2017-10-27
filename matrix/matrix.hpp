#pragma once

#include <cstdint>
#include <string>
#include <vector>

class matrix 
{
public: // Typedef
	using int_type = int32_t;

public: // Functions
	/** 
   	 *	Loads a matrix in binary format into the data_ container.
	 *
	 *	@param filename The file containing the data
	 *	@param dimension The matrix is of size NxN, dimension = N
	 */
	void read_binary(std::string const& filename, int dimension);

	/**
 	 *	Loads a matrix in human readable form into the data_ container.
	 *
	 *	@param filename The file containing the data
	 *	@param dimension The matrix is of size NxN, dimension = N
 	 */
	void read_data(std::string const& filename, int dimension);

private: // Members
	std::vector <int_type> data_;
};
