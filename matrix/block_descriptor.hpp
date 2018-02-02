#pragma once

template <typename T>
struct BlockDescriptor
{
	BlockDescriptor(T* matrix, int blockX, int blockY)
		: matrix{matrix}
		, x{blockX}
		, y{blockY}
	{
	}

	T* matrix;
	int x;
	int y;
};
