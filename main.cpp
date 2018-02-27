#include "mpi/mpi_core.hpp"
#include "program_options.hpp"
#include "parallel_matrix.hpp"
#include "debug.hpp"

#include "exercises/3_4.hpp"
#include "exercises/4_2.hpp"
#include "exercises/4_3.hpp"
#include "exercises/4_4.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	// Parameters
    auto maybe_options = parse_arguments(argc, argv);
#ifndef __INTEL_COMPILER
    if (!maybe_options)
        return 0;

    auto options = maybe_options.get();
#else
    auto options = maybe_options;
#endif

    // The MPI context handles everything MPI world related.
    Mpi::Context ctx{options.root};

#if defined(DO_3_4)
    std::cout << "3_4\n";
    exercise_3_4(ctx, options);
    // Alle enthalten (leider) schon 4.5:
#elif defined(DO_4_2)
    std::cout << "4_2\n";
    exercise_4_2(ctx, options);
#elif defined(DO_4_3)
    std::cout << "4_3\n";
    exercise_4_3(ctx, options);
#elif defined(DO_4_4)
    std::cout << "4_4\n";
    exercise_4_4(ctx, options);
#elif defined(TRAFFIC_BENCH)
    std::cout << "traffic bench\n";
#elif defined(STRIPE_BENCH)
    std::cout << "stripe bench\n";
    //Mpi::SharedMatrixFile file{MPI_COMM_WORLD, "temp", Mpi::FileStreamDirection::Write, 1, 1 * 1024u * 1024u, ctx.size()};
#else
    std::cout << "final\n";
    ParallelContext mmContext{ctx, options};
    mmContext.imbue_views();
    mmContext.perform();
    mmContext.dump_stamps_sync();
#endif

	return 0;
}
