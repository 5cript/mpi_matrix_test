#pragma once

#include "../matrix/matrix.hpp"

struct BlockDescriptor
{
	BlockDescriptor(Matrix* matrix, int blockX, int blockY);

	Matrix* matrix;
	int x;
	int y;
};
