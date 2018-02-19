#include "4_2.hpp"

void exercise_4_2(Mpi::Context& ctx, ProgramOptions options)
{
    if (options.humanReadableOutput)
    {
        std::cout << "human readable output not supported for MPI I/O.\n";
        options.humanReadableOutput = false;
    }

    ParallelContext mmContext{ctx, options};
    mmContext.use_write_at_all(false);
    mmContext.perform();
    mmContext.dump_stamps_sync();
}
