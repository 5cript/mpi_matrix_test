#include "matrix_loader.hpp"
#include "matrix_vector_view.hpp"

#include "../container_print.hpp"
#include "../mpi/group.hpp"
#include "../mpi/types.hpp"

#include <algorithm>
#include <iostream>
#include <type_traits>

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

    //std::cout << instanceId << ": [" << left << ", " << right << "]\n";
}
//#####################################################################################################################
MatrixStorage::MatrixStorage(
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
void MatrixStorage::load_local(Mpi::SharedMatrixFile& left, Mpi::SharedMatrixFile& right)
{	
	leftVector_ = std::make_unique <MatrixVector> (blockDimension_, blocksPerRC_);
	rightVector_ = std::make_unique <MatrixVector> (blockDimension_, blocksPerRC_);
 
	for (auto const& load : instruction_.loadResponsibility)
	{
		auto y = load / blocksPerRC_;
		auto x = load % blocksPerRC_;	

		auto leftLoadedMatrix = leftVector_->get(x);
		//leftLoadedMatrix->resize(blockDimension_);
		left.readBlock <MatrixVectorView>({&leftLoadedMatrix, x, y}, blockDimension_ * blocksPerRC_);

		auto rightLoadedMatrix = rightVector_->get(y);
		//rightLoadedMatrix->resize(blockDimension_);
        right.readBlock <MatrixVectorView>({&rightLoadedMatrix, x, y}, blockDimension_ * blocksPerRC_);
	}
}
//---------------------------------------------------------------------------------------------------------------------
MatrixVector* MatrixStorage::left()
{
    return leftVector_.get();
}
//---------------------------------------------------------------------------------------------------------------------
MatrixVector* MatrixStorage::right()
{
    return rightVector_.get();
}
//---------------------------------------------------------------------------------------------------------------------
void MatrixStorage::share_blocks()
{
	Mpi::WorldGroup world;
    auto pullSide = [this, &world](auto& vector, auto const& blocks)
    {
        auto subnet = blocks;
        auto responsibility = *instruction_.loadResponsibility.begin();
        subnet.push_back(responsibility);

        // Sort numbers to have identical subcommunicators
        std::sort(std::begin(subnet), std::end(subnet));

        // Find out which vector index is the one with the loaded chunk
        auto respIndex = std::find(std::begin(subnet), std::end(subnet), responsibility) - std::begin(subnet);

        //std::cout << "sharing_net: " << subnet << " LR: " << respIndex << "\n";

        // Create Subcommunicator:
        Mpi::SubGroup distributionNet{world, subnet};
        auto subcom = distributionNet.create_communicator();

        // Spread chunks over net.
        MPI_Allgather
        (
            vector->get(respIndex).get_line(0),
            //vector->get(0).get_line(0),
            blockDimension_ * blockDimension_,
            Mpi::ConvertToMpiType <matrix_value_type>::value,
            vector->get(0).get_line(0),
            blockDimension_ * blockDimension_,
            Mpi::ConvertToMpiType <matrix_value_type>::value,
            subcom
        );

        MPI_Comm_free(&subcom);
    };

    pullSide(leftVector_, instruction_.left);
    pullSide(rightVector_, instruction_.right);
}
//#####################################################################################################################
