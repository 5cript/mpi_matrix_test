#include "matrix/matrix.hpp"
#include "bench/benchhelp.hpp"
#include "mpi/mpi_core.hpp"

#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
	MpiCore ctx;
	std::cout << "id: " << ctx.id() << " size: " << ctx.size() << "\n";
	
	return 0;
}
