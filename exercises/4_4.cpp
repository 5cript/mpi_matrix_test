#include "4_4.hpp"

void exercise_4_4(Mpi::Context& ctx, ProgramOptions options)
{
    if (options.humanReadableOutput)
    {
        //std::cout << "human readable output not supported for MPI I/O.\n";
        options.humanReadableOutput = false;
    }

    ParallelContext mmContext{ctx, options};
    mmContext.imbue_views();
    mmContext.perform();
    mmContext.dump_stamps_sync();
}
