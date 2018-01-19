#include "block_descriptor.hpp"

BlockDescriptor::BlockDescriptor(Matrix* matrix, int blockX, int blockY)
	: matrix{matrix}
	, x{blockX}
	, y{blockY}
{

}
