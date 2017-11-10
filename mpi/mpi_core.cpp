#include "mpi_core.hpp"

namespace Mpi
{
//####################################################################################################################
	Context::Context(int root)
		: root_{root}
	{
		MPI_Init(nullptr, nullptr);
	}
//---------------------------------------------------------------------------------------------------------------------
	Context::~Context()
	{
		MPI_Finalize();
	}
//---------------------------------------------------------------------------------------------------------------------
	int Context::id() const
	{
		int id;
		MPI_Comm_rank(MPI_COMM_WORLD, &id);
		return id;
	}
//---------------------------------------------------------------------------------------------------------------------
	int Context::size() const
	{
		int size;
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		return size;
	}
//---------------------------------------------------------------------------------------------------------------------
	int Context::root() const
	{
		return root_;
	}
//---------------------------------------------------------------------------------------------------------------------
	bool Context::is_root() const
	{
		return id() == root_;
	}
//#####################################################################################################################
}
