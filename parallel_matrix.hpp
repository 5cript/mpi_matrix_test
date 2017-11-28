#pragma once

#include "program_options.hpp"
#include "mpi/mpi_core.hpp"

int parallel_multiplication(Mpi::Context& ctx, ProgramOptions const& options);
