#include "3_4.hpp"

#include "mpi/communicator.hpp"
#include "matrix/matrix.hpp"
#include "matrix/matrix_partition.hpp"
#include "matrix/block_list.hpp"
#include "bench/benchhelp.hpp"

#include <stdexcept>

using namespace std::string_literals;

void exercise_3_4(Mpi::Context& ctx, ProgramOptions const& options)
{
    TimeStampCollection stamps;
    stamps.add("Start");

    Matrix lhs{};
    Matrix rhs{};

    // Root reads matrices.
    if (ctx.is_root())
    {
        if (options.humanReadableInput)
        {
            lhs.read_data(options.leftMatrix, options.dimension);
            rhs.read_data(options.rightMatrix, options.dimension);
        }
        else
        {
            lhs.read_binary(options.leftMatrix, options.dimension);
            rhs.read_binary(options.rightMatrix, options.dimension);
        }
    }
    else
    {
        lhs.resize(options.dimension);
        rhs.resize(options.dimension);
    }

    // spread matrices to children.
    Mpi::Communicator communicator{&ctx};
    communicator.broadcast <Matrix::value_type> (lhs.data(), lhs.data_size());
    communicator.broadcast <Matrix::value_type> (rhs.data(), rhs.data_size());

    auto blockWidth = MatrixPartition::optimal_partitioning(options.dimension, ctx.size());
    auto div = options.dimension/blockWidth;
    auto rest = (options.dimension/blockWidth)*(options.dimension/blockWidth) - ctx.size();

    if (rest != 0)
        throw std::invalid_argument{std::string{"Cannot find optimal partitioning with mpi instance count of "} + std::to_string(ctx.size()) + " and dimension " + std::to_string(options.dimension)};

    Matrix resultBlock{blockWidth};
    auto resultBlockView = MatrixPartition{&resultBlock, 1, ctx.size()};

    auto leftPartition = MatrixPartition{&lhs, div, ctx.size()};
    auto rightPartition = MatrixPartition{&rhs, div, ctx.size()};

    // root needs the entire matrix.
    std::vector <Matrix::value_type> overallResult;
    if (ctx.is_root())
        overallResult.resize(options.dimension*options.dimension);

    auto multiply = [&]()
    {
        auto y = ctx.id() / div;
        auto x = ctx.id() % div;

        for (int i = 0; i != div; ++i)
        {
            leftPartition.aquire(i, y).accum_multiply(
                rightPartition.aquire(x, i), resultBlockView.aquire(0, 0) // rP(x, y)
            );
        }
    };
    resultBlock.clear();
    multiply();

    communicator.gather(resultBlock.data(), ctx.is_root() ? overallResult.data() : nullptr, resultBlock.data_size());

    MPI_Barrier(MPI_COMM_WORLD);

    if (ctx.is_root())
    {
        BlockList blocks{&overallResult, blockWidth, options.dimension};
        blocks.save_matrix(options.resultMatrix);
    }

    stamps.add("End");
    if (ctx.is_root())
    {
        stamps.dump();
    }
}
