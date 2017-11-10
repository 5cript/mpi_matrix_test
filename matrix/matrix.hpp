#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Matrix 
{
public: // Typedef
	using value_type = int32_t;

public: // Functions
	Matrix() = default;
	~Matrix() = default;
	
	/** 
	 *	Creates and loads a BINARY matrix.
	 *	Calls read_binary(binaryFile, dimension);
	 */
	Matrix(std::string const& binaryFile, int dimension);

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

	/** 
	 *	Writes the matrix into a file.
	 *
	 *	@param filename The file location.
	 */
	bool write_data(std::string const& filename);

	/**
 	 *	Serial Matrix multiplication.
	 *	Non commutative.
	 */
	Matrix operator*(Matrix const& other);

	/** 
 	 *	Provides access to internal values.
	 */
	value_type* data();

	/** 
	 *	Set the size of the matrix.
	 */
	void resize(int dimension);

	/** 
 	 *	@return dimension * dimension
	 */
	std::size_t data_size() const;

	/// Debug function, to show matrix contents (up to a max)
	void print(int maxValues = 100);

	/// Disallow copying. Never copy 4 gigs casually anyway
	Matrix& operator=(Matrix const&) = delete;

	/// Non moveable for now
	Matrix& operator=(Matrix&&) = delete;

	/// Disallow copying. Never copy 4 gigs casually anyway
	Matrix(Matrix const&) = delete;

	Matrix(Matrix&&) = default;

private: // Members
	std::vector <value_type> data_;
	int dimension_;
};
