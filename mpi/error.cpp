#include "error.hpp"
#include "mpi_core.hpp"

#include <iostream>

void print_error(int errorCode)
{
	if (errorCode == 0)
		return;

	char buffer[1024];
	int len = 1024;
	MPI_Error_string(errorCode, buffer, &len);
	std::cerr << "Error: " << buffer << "\n";
}
