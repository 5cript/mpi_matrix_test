#pragma once

#include "mpi_core.hpp"
#include "types.hpp"
#include "error.hpp"

namespace Mpi
{
	class Communicator
	{
	public:
		Communicator(Context* context, MPI_Comm com = MPI_COMM_WORLD);
		~Communicator();

		MPI_Comm get_handle() const;

		/**
	 	 *	Broadcast data over to all instances.
		 */
		template <typename T>
		void broadcast(T* data, int bufferSize)
		{
			int res = MPI_Bcast(
				reinterpret_cast <void*> (data),
				bufferSize,
				TO_MPI_TYPE(T),
				context_->root(),
				com_
			);
			print_error(res);
		}

		template <typename T>
		void gather(T* sendData, T* recvBuffer, int sendBufferSize)
		{
			int res = MPI_Gather(
				sendData,
				sendBufferSize,
				TO_MPI_TYPE(T),
				recvBuffer,
				sendBufferSize,
				TO_MPI_TYPE(T),
				context_->root(),
				com_
			);
			print_error(res);
		}
	private:
		Context* context_;
		MPI_Comm com_;
	};
}
