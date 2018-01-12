#pragma once

#include "matrix.hpp"

#include <vector>

/**
 *	A gather operation gathers the data as a sequence of blocks.
 *	This class can demangle it back to a matrix, without copying.
 */
class BlockList
{
public:
	BlockList(std::vector <Matrix::value_type>* blocks, int blockDimension, int overallDimension);

	/**
 	 *	Prints a block in human readable form.
	 */
	void print_block(int x, int y);

	bool save_matrix(std::string const& filename, bool humanReadable = true);

	/**
 	 *	Saves the matrix as a sequence of block matrices. DEBUGGING ONLY. HUMAN READABLE ONLY.
	 */
	bool save_block_sequence(std::string const& filename);

private:
	/// This container is encoded in: block_x_major, x_major.
	std::vector <Matrix::value_type>* blocks_;
	int blockDimension_;
	int overallDimension_;
};
