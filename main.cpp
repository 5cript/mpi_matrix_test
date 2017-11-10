#include "mpi/mpi_core.hpp"
#include "program_options.hpp"
#include "parallel_matrix.hpp"

#include <iostream>
#include <chrono>

#include <thread>

static constexpr auto root = 0;
static constexpr auto dimension = 100;

//#define DO_TRANSMIT

int main(int argc, char** argv)
{
	// Parameters
	auto options = parse_arguments(argc, argv);
	if (!options)
		return 1;

	// Actual Job
	Mpi::Context ctx{root};	
	parallelMul(ctx, options.get().dimension);	

	return 0;
}
