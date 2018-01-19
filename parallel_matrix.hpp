#pragma once

#include "program_options.hpp"
#include "mpi/mpi_core.hpp"
#include "matrix/forward.hpp"

#include <set>
#include <utility>

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
