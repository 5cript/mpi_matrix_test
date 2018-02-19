#pragma once

#include "program_options.hpp"
#include "mpi/mpi_core.hpp"
#include "matrix/forward.hpp"

#include "bench/benchhelp.hpp"
#include "container_print.hpp"
#include "matrix/matrix_loader.hpp"

#include "mpi/communicator.hpp"
#include "mpi/group.hpp"
#include "mpi/shared_file.hpp"

#include "matrix/matrix.hpp"
#include "matrix/matrix_partition.hpp"
#include "matrix/block_list.hpp"
#include "matrix/matrix_loader.hpp"

#include <set>
#include <utility>

class ParallelContext
{
public:
	ParallelContext(Mpi::Context& ctx, ProgramOptions const& options);

	ParallelContext& operator=(ParallelContext const&) = delete;
	ParallelContext& operator=(ParallelContext&&) = delete;
	ParallelContext(ParallelContext const&) = delete;
	ParallelContext(ParallelContext&&) = delete;

	void legacy_broadcast();
	void do_multiplication();

    void perform();

	/**
 	 *	Load the own matrix blocks for the given cycle.
	 */
    void load_blocks(int cycle = 0);

    void use_write_at_all(bool waa);

    void imbue_views();

    void dump_stamps_sync();

private:
	void calculate_partitioning();
	void show_partitioning();

private:
	Mpi::Context& ctx;
	ProgramOptions const& options;
	Mpi::Communicator communicator;
	TimeStampCollection stamps;
	int dimension;
	int blockWidth;
	int div; // blocks per row or column
    int x; // my x block coordinate
    int y; // my y block coordinate
    bool writeAtAll;
	Mpi::SharedMatrixFile leftSharedFile_;
	Mpi::SharedMatrixFile rightSharedFile_;
	Mpi::SharedMatrixFile resultSharedFile_;
    std::unique_ptr <MatrixStorage> storage_;
};

/**
 *	Get the coordinates of this instance. There may be more blocks in total than instances.
 *	This is what the offset parameter is for. The offset should be a multiple of ctx.size().
 */
std::pair <int, int> getMyCoordinates(Mpi::Context& ctx, int blocksPerRow, int offset);

/**
 *  Performs a parallelized matrix multiplication on matrices that are loaded based
 *  on the given options. The matrices will be reinterpreted as block matrices and 
 *  then multiplied block by block in different instances.
 */
int parallel_multiplication(Mpi::Context& ctx, ProgramOptions const& options);

/**
 *  Loads the matrices from based on options.
 */
int load_matrices(Mpi::Context& ctx, ProgramOptions const& options, Matrix& lhs, Matrix& rhs);

/**
 *  Loads the matrices from based on options.
 */
std::set <int> optimal_instances(int dimension);

void save_intermediary(Matrix const& matrix, Mpi::Context& ctx, ProgramOptions const& options, int suffix);
