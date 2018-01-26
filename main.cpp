#include "mpi/mpi_core.hpp"
#include "program_options.hpp"
#include "parallel_matrix.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	// Parameters
	auto options = parse_arguments(argc, argv);
	if (!options)
		return 0;

    // The MPI context handles everything MPI world related.
	Mpi::Context ctx{options.get().root};   

	// Actual Job
	ParallelContext mmContext{ctx, options.get()};
	mmContext.do_multiplication();
    //parallel_multiplication(ctx, options.get());

	return 0;
}
