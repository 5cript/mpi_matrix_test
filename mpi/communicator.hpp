#pragma once

#include "mpi_core.hpp"
#include "types.hpp"

namespace Mpi
{
	class Communicator
	{
	public:
		Communicator(Context* context);
	
		/**
	 	 *	Broadcast data over to all instances.
		 */
		template <typename T>
		void broadcast(T* data, int count)
		{
			MPI_Bcast(
				reinterpret_cast <void*> (data), 
				count, 
				ConvertToMpiType <T>::value,
				context_->root(),
				MPI_COMM_WORLD
			);
		}
	private:
		Context* context_;
	};
}
