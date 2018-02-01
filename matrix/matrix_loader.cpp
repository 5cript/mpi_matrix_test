#include "matrix_loader.hpp"
#include "../container_print.hpp"

#include "../mpi/group.hpp"

#include <algorithm>
#include <iostream>

//#####################################################################################################################
struct StepGenerator
{
	StepGenerator(int offset, int step)
		: offset_{offset}
		, step_{step}
		, state_{0}
	{}

	int operator()()
	{
		int ret = offset_ + state_;
		state_ += step_;
		return ret;
	}

private:
	int offset_;
	int step_;
	int state_;
};
//---------------------------------------------------------------------------------------------------------------------
PullInstruction::PullInstruction(int instanceId, int instanceCount, int blocksPerRC)
	: left(blocksPerRC) // intentional braces
	, right(blocksPerRC) // intentional braces
	, loadResponsibility{}
{
	auto y = instanceId / blocksPerRC;
	auto x = instanceId % blocksPerRC;	

	std::generate_n(std::begin(left), blocksPerRC, StepGenerator{blocksPerRC * y, 1});
	std::generate_n(std::begin(right), blocksPerRC, StepGenerator{x, blocksPerRC});

	auto eraseSelf = [&](std::vector <int>& container) {
		container.erase(std::remove_if(
			std::begin(container), 
			std::end(container), 
			[&](auto elem){		
				bool myResponsibility = (
					elem % instanceCount == instanceId
				);
				if (myResponsibility)
					loadResponsibility.insert(elem);
				return myResponsibility;
			}
		), std::end(container));
	};

	// erase self from retrieval, since I have my own block already.
	eraseSelf(left);
	eraseSelf(right);

	std::cout << instanceId << ": [" << left << ", " << right << "]\n";
}
//#####################################################################################################################
MatrixLoader::MatrixLoader(
	Mpi::Communicator* communicator, 
	int instanceId, 
	int instanceCount, 
	int blocksPerRC,
	int blockDimension
)
	: communicator_{communicator}
	, instruction_{instanceId, instanceCount, blocksPerRC}
	, leftVector_{}
	, rightVector_{}
	, instanceId_{instanceId}
	, instanceCount_{instanceCount}
	, blocksPerRC_{blocksPerRC}
	, blockDimension_{blockDimension}
{
}
//---------------------------------------------------------------------------------------------------------------------
void MatrixLoader::load_local(Mpi::SharedMatrixFile& left, Mpi::SharedMatrixFile& right)
{	
	leftVector_ = std::make_unique <MatrixVector> (blocksPerRC_);
	rightVector_ = std::make_unique <MatrixVector> (blocksPerRC_);
 
	for (auto const& load : instruction_.loadResponsibility)
	{
		auto y = load / blocksPerRC_;
		auto x = load % blocksPerRC_;	

		auto* leftLoadedMatrix = leftVector_->get(x);
		leftLoadedMatrix->resize(blockDimension_);
		left.readBlock({leftLoadedMatrix, x, y}, blockDimension_ * blocksPerRC_);

		auto* rightLoadedMatrix = rightVector_->get(y);
		rightLoadedMatrix->resize(blockDimension_);
		right.readBlock({leftLoadedMatrix, x, y}, blockDimension_ * blocksPerRC_);
	}
}
//---------------------------------------------------------------------------------------------------------------------
void MatrixLoader::share_blocks()
{
	Mpi::WorldGroup world;
    auto pullSide = [this, &world]()
	{
        //Mpi::SubGroup distributionNet{world, subGroupIds};

			
	};
	/*	
	for (int i = 0; i != instruction_.left; ++i)
	{
		 
	}
	*/
}
//#####################################################################################################################
