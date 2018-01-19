#pragma once

#include "forward.hpp"

#include <cstdint>
#include <string>
#include <vector>

/**
 *	A matrix is the general data holding container which provides load and save capabilities, as well
 *	as direct data access methods.
 *
 * The internal data representation is row-major.
 */
class Matrix
{
public: // Friends
	friend MatrixBlock;

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
 	 *	Creates an empty matrix of a specific size.
	 */
	Matrix(int dimension);

	/**
 	 *	Resets all matrix elements to 0.
	 */
	void clear();

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
	bool write_binary(std::string const& filename) const;

	/**
	 *	Writes the matrix into a file.
	 *
	 *	@param filename The file location.
	 */
	bool write_data(std::string const& filename) const;

	/**
 	 *	Serial Matrix multiplication.
	 *	Non commutative.
	 */
	Matrix operator*(Matrix const& other);

	/**
 	 *	Matrix addition.
	 */
	Matrix operator+(Matrix const& other);

	/**
 	 *	Provides access to internal values.
	 */
	value_type* data();

	/**
 	 *	Get a data offset to the specified line.
	 */
	value_type* get_line(int y);

	/**
	 *	Set the size of the matrix.
	 */
	void resize(int dimension);

	/**
	 *	Returns the dimension of the matrix.
	 */
	int dimension() const;

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

	/**
	 *  Generates a random matrix
	 */
    friend Matrix generate_random_matrix(int dimension);

private: // Members
	std::vector <value_type> data_;
	int dimension_;
};

Matrix generate_random_matrix(int dimension);
