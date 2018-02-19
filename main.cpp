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
    if (!maybe_options)
        return 0;

    auto options = maybe_options.get();

    // The MPI context handles everything MPI world related.
    Mpi::Context ctx{options.root};

#if defined(DO_3_4)
    //exercise_3_4(ctx, options);
#endif

    // Alle enthalten (leider) schon 4.5:
#if defined(DO_4_2)
    //exercise_4_2(ctx, options);
#elif defined(DO_4_3)
    //exercise_4_3(ctx, options);
#elif defined(DO_4_4)
    exercise_4_4(ctx, options);
#else
    ParallelContext mmContext{ctx, options};
    mmContext.imbue_views();
    mmContext.perform();
    mmContext.dump_stamps_sync();
#endif

	return 0;
}
