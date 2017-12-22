#pragma once

#include "mpi_core.hpp"
#include "error.hpp"

#include <vector>
#include <iostream>
#include <iomanip>

namespace Mpi
{
	class Group
	{	
	public:
		Group() noexcept = default;
		virtual ~Group()
		{
			MPI_Group_free(&group_);
		}

		Group(Group const&) = delete;
		Group(Group&&) = default;
		Group& operator=(Group const&) = delete;
		Group& operator=(Group&&) = default;

		MPI_Group get() noexcept
		{
			return group_;
		}

		MPI_Comm create_communicator(int tag = 0) noexcept
		{
			MPI_Comm result;
			//MPI_Comm_create(MPI_COMM_WORLD, group_, &result);
			MPI_Comm_create_group(MPI_COMM_WORLD, group_, tag, &result);
			return result;
		}

	protected:
		MPI_Group group_;
	};

	class SubGroup : public Group
	{
	public:
		SubGroup(Group& parentGroup, std::vector <int> const& arr) noexcept
		{
			int res = MPI_Group_incl(parentGroup.get(), arr.size(), arr.data(), &group_);
			print_error(res);
		}
	};

	class WorldGroup : public Group
	{
	public:
		WorldGroup() noexcept
		{
			int res = MPI_Comm_group(MPI_COMM_WORLD, &group_);
			print_error(res);
		}
	};
}
