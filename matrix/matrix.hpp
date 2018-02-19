#pragma once

#include "forward.hpp"
#include "matrix_value_type.hpp"

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Convenience class for accessing a matrix with operator[].
 */
class MatrixSubscriptProxy
{
public:
    friend Matrix;

public:
    matrix_value_type& operator[](int y);

private:
    MatrixSubscriptProxy(Matrix* owner, int x);

private:
    Matrix* owner_;
    int x_;
};

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
    friend MatrixSubscriptProxy;

public: // Typedef
	using value_type = matrix_value_type;

public: // Functions
	Matrix() = default;
	~Matrix() = default;

	/**
	 *	Creates and loads a BINARY matrix.
	 *	Calls read_binary(binaryFile, dimension);
	 */
	Matrix(std::string const& binaryFile, int dimension);

	/**
 	 *	Creates an empty matrix of a specific size (WARNING does allocation).
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
     * @brief operator [] to access individual cells. Use is discouraged
     * @param y
     * @return A subscript proxy to go further down on x values.
     */
    MatrixSubscriptProxy operator[](int x);

    /**
     * @brief Faster alternative to operator[][].
     * @return
     */
    value_type& at(int x, int y)
    {
        return *(&data_.front() + x + y * dimension_);
    }

	/**
	 *  Generates a random matrix
	 */
    friend Matrix generate_random_matrix(int dimension);

private: // Members
	std::vector <value_type> data_;
	int dimension_;
};

Matrix generate_random_matrix(int dimension);
