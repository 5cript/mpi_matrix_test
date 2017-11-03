#include "mpi_core.hpp"

//#####################################################################################################################
MpiCore::MpiCore()
{
	MPI_Init(nullptr, nullptr);
}
//---------------------------------------------------------------------------------------------------------------------
MpiCore::~MpiCore()
{
	MPI_Finalize();
}
//---------------------------------------------------------------------------------------------------------------------
int MpiCore::rank() const
{
	int id;
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	return id;
}
//---------------------------------------------------------------------------------------------------------------------
int MpiCore::size() const
{
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	return size;
}
//#####################################################################################################################
