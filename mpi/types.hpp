#pragma once

#include "mpi_core.hpp"

namespace Mpi
{
	template <typename T>
	struct ConvertToMpiType {};
}

#ifndef __INTEL_COMPILER
#define MPI_TEST_DECLARE_CONVERSION(CT, MT) \
template <> \
struct ConvertToMpiType <CT> \
{ \
	static constexpr const MPI_Datatype value = MT; \
}
#else
#define MPI_TEST_DECLARE_CONVERSION(CT, MT) \
template <> \
struct ConvertToMpiType <CT> \
{ \
	MPI_Datatype value = MT; \
}
#endif

namespace Mpi
{
	MPI_TEST_DECLARE_CONVERSION(short, MPI_SHORT);
	MPI_TEST_DECLARE_CONVERSION(int, MPI_INT);
	MPI_TEST_DECLARE_CONVERSION(long int, MPI_LONG);
	MPI_TEST_DECLARE_CONVERSION(long long, MPI_LONG_LONG);
	MPI_TEST_DECLARE_CONVERSION(unsigned char, MPI_UNSIGNED_CHAR);
	MPI_TEST_DECLARE_CONVERSION(unsigned short, MPI_UNSIGNED_SHORT);
	MPI_TEST_DECLARE_CONVERSION(unsigned int, MPI_UNSIGNED);
	MPI_TEST_DECLARE_CONVERSION(unsigned long, MPI_UNSIGNED_LONG);
	MPI_TEST_DECLARE_CONVERSION(unsigned long long, MPI_UNSIGNED_LONG_LONG);
	MPI_TEST_DECLARE_CONVERSION(float, MPI_FLOAT);
	MPI_TEST_DECLARE_CONVERSION(double, MPI_DOUBLE);
	MPI_TEST_DECLARE_CONVERSION(long double, MPI_LONG_DOUBLE);
	MPI_TEST_DECLARE_CONVERSION(char, MPI_BYTE);
}

#ifndef __INTEL_COMPILER
#   define TO_MPI_TYPE(X) Mpi::ConvertToMpiType <X>::value
#else
#   define TO_MPI_TYPE(X) Mpi::ConvertToMpiType <X>{}.value
#endif
