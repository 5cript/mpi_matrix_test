#pragma once

#include <mpi.h>

#include "error.hpp"

namespace Mpi
{
	class Context
	{
	public:
		/**
	 	 *	Creates the context class.
		 *	@param root Determines which process shall act out the root process.
		 */
		Context(int root = 0);
		~Context();

		/**
	 	 *	Who am i? Retrieves the id of this MPI instance
		 */
		int id() const;

		/**
	 	 *	How much instances are there?
		 */
		int size() const;

		/**
		 *	Who is root?
		 */
		int root() const;

		/**
		 *	Is this instance root?
		 */
		bool is_root() const;

	private:
		int root_;
	};
}
