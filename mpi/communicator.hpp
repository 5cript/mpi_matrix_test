#pragma once

#include "mpi_core.hpp"
#include "types.hpp"

namespace Mpi
{
	class Communicator
	{
	public:
		Communicator(Context* context, MPI_Comm com = MPI_COMM_WORLD);
	
		/**
	 	 *	Broadcast data over to all instances.
		 */
		template <typename T>
		void broadcast(T* data, int bufferSize)
		{
			MPI_Bcast(
				reinterpret_cast <void*> (data), 
				bufferSize, 
				ConvertToMpiType <T>::value,
				context_->root(),
				com_
			);
		}

		template <typename T>
		void gather(T* sendData, T* recvBuffer, int sendBufferSize)
		{		
			MPI_Gather(
				sendData,
				sendBufferSize,
				ConvertToMpiType <T>::value,
				recvBuffer,
				sendBufferSize,
				ConvertToMpiType <T>::value,
				context_->root(),
				com_
			);
		}
	private:
		Context* context_;
		MPI_Comm com_;
	};
}
