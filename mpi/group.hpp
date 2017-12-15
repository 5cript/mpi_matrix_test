#pragma once

#include "mpi_core.hpp"

#include <vector>

namespace Mpi
{
	class Group
	{	
	public:
		Group() noexcept = default;
		virtual ~Group() = default;

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
			MPI_Group_incl(parentGroup.get(), arr.size(), arr.data(), &group_);
		}

	protected:
		MPI_Group group_;
	};

	class WorldGroup : public Group
	{
	public:
		WorldGroup() noexcept
		{
			MPI_Comm_group(MPI_COMM_WORLD, &group_);
		}
	};
}
