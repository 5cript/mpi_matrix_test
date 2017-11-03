#pragma once

#include <mpi.h>

class MpiCore
{
public:
	MpiCore();
	~MpiCore();

	int rank() const;
	int size() const;
};
