#pragma once

#include "mpi_core.hpp"
#include "communicator.hpp"
#include "types.hpp"

namespace Mpi
{
	enum class FileStreamDirection
	{
		Read,
		Write
	};

	class SharedFile
	{
	public:
		SharedFile(MPI_Comm communicator, std::string const& filename, FileStreamDirection openMode);
		~SharedFile();

		template <typename T>
		void write(int id, T const* data, std::size_t count, std::size_t offset)
		{
			int res = MPI_File_write_at_all(handle_, offset, data, count, ConvertToMpiType <T>::value, MPI_STATUS_IGNORE);
			//int res = MPI_File_write_at(handle_, offset, data, count, ConvertToMpiType <T>::value, MPI_STATUS_IGNORE);
			print_error(res);
		}
		
		SharedFile& operator=(SharedFile const&) = delete;
		SharedFile& operator=(SharedFile&&) = delete;		

		SharedFile(SharedFile const&) = delete;
		SharedFile(SharedFile&&) = delete;

	private:
		MPI_File handle_;
	};	
}
