#include "4_3.hpp"

void exercise_4_3(Mpi::Context& ctx, ProgramOptions options)
{
    if (options.humanReadableOutput)
    {
        //std::cout << "human readable output not supported for MPI I/O.\n";
        options.humanReadableOutput = false;
    }

    ParallelContext mmContext{ctx, options};
    mmContext.use_write_at_all(true);
    mmContext.perform();
    mmContext.dump_stamps_sync();
}
