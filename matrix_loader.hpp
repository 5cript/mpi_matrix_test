#pragma once

#include <vector>

struct PullInstruction
{
public:
	PullInstruction(int instanceId, int instanceCount, int blocksPerRC);

//private:
	std::vector <int> left_; // loaded blocks for left matrix.
	std::vector <int> right_; // loaded blocks for right matrix.
};
