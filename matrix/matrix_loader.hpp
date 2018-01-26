#pragma once

#include "matrix_vector.hpp"

#include "../mpi/communicator.hpp"
#include "../mpi/shared_file.hpp"

#include <vector>
#include <set>
#include <memory>

struct PullInstruction
{
public:
	PullInstruction(int instanceId, int instanceCount, int blocksPerRC);

//private:
	std::vector <int> left; // loaded blocks for left matrix.
	std::vector <int> right; // loaded blocks for right matrix.
	/// Load responsibilities are for both sides.
	std::set <int> loadResponsibility;
};

/**
 *	This class is designed to be used once per multiplication task of a client.
 *	Data is shared multiple times, when need arises, since some instances have to do work multiple times.
 */
class MatrixLoader
{
public:
	MatrixLoader(
		Mpi::Communicator* communicator, 
		int instanceId, 
		int instanceCount, 
		int blocksPerRC,
		int blockDimension
	);

public:
	void load_local(Mpi::SharedMatrixFile& left, Mpi::SharedMatrixFile& right);
	void share_blocks();

private:
	Mpi::Communicator* communicator_;
	PullInstruction instruction_;
	std::unique_ptr <MatrixVector> leftVector_;
	std::unique_ptr <MatrixVector> rightVector_;
	int instanceId_;
	int instanceCount_;
	int blocksPerRC_;
	int blockDimension_;
};
