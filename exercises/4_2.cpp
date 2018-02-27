#include "4_2.hpp"

void exercise_4_2(Mpi::Context& ctx, ProgramOptions options)
{
    if (options.humanReadableOutput)
    {
        std::cout << "human readable output not supported for MPI I/O.\n";
        options.humanReadableOutput = false;
    }

    auto perform_once = [&](int size)
    {
        std::cout << "share for: " << size << "\n";
        options.dimension = size;
        try
        {
            ParallelContext mmContext{ctx, options};
            mmContext.use_write_at_all(false);
            mmContext.load_blocks();
            mmContext.dump_stamps_sync();
        }
        catch(std::exception const& exc)
        {
            std::cout << exc.what();
        }
    };

    perform_once(10);
    perform_once(100);
    perform_once(1000);
    perform_once(30000);
}
