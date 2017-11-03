#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Matrix 
{
public: // Typedef
	using int_type = int32_t;

public: // Functions
	Matrix() = default;
	~Matrix() = default;

	/** 
   	 *	Loads a matrix in binary format into the data_ container.
	 *
	 *	@param filename The file containing the data
	 *	@param dimension The matrix is of size NxN, dimension = N
	 */
	bool read_binary(std::string const& filename, int dimension);

	/**
 	 *	Loads a matrix in human readable form into the data_ container.
	 *
	 *	@param filename The file containing the data
	 *	@param dimension The matrix is of size NxN, dimension = N
 	 */
	bool read_data(std::string const& filename, int dimension);

	/** 
	 *	Writes the matrix into a file.
	 *
	 *	@param filename The file location.
	 */
	bool write_binary(std::string const& filename);

	void write_matrix(std::string const& filename);

	/**
 	 *	Serial Matrix multiplication.
	 */
	Matrix operator*(Matrix const& other);

	/// Debug function, to show matrix contents (up to a max)
	void print(int maxValues = 100);

	/// Disallow copying. Never copy 4 gigs casually anyway
	Matrix& operator=(Matrix const&) = delete;

	/// Non moveable for now
	Matrix& operator=(Matrix&&) = delete;

	/// Disallow copying. Never copy 4 gigs casually anyway
	Matrix(Matrix const&) = delete;

	/// Non moveable for now
	Matrix(Matrix&&) = delete;

private: // Members
	std::vector <int_type> data_;
	int dimension_;
};
