#include "matrix_loader.hpp"

#include <algorithm>

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

PullInstruction::PullInstruction(int instanceId, int instanceCount, int blocksPerRC)
	: left_(blocksPerRC)
	, right_(blocksPerRC)
{
	auto y = instanceId / blocksPerRC;
	auto x = instanceId % blocksPerRC;	

	std::generate_n(std::begin(left_), blocksPerRC, StepGenerator{blocksPerRC * x, 1});
	std::generate_n(std::begin(right_), blocksPerRC, StepGenerator{y, blocksPerRC});

	
}
