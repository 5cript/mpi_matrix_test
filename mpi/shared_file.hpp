	#pragma once

#include "mpi_core.hpp"
#include "communicator.hpp"
#include "types.hpp"

#include "../matrix/block_descriptor.hpp"

namespace Mpi
{
	enum class FileStreamDirection
	{
		Read,
		Write
	};

	class SharedMatrixFile
	{
	public:
		SharedMatrixFile(MPI_Comm communicator, std::string const& filename, FileStreamDirection openMode);
		~SharedMatrixFile();

		/**
 		 *	Read a block from a matrix.
		 *
		 *	@warning The matrix must have been allocated beforehand.
		 */		
		void readBlock(BlockDescriptor descriptor, int totalDimension)
		{
			auto readLine = [this, &descriptor, &totalDimension](int yb) -> int {
				auto* line = descriptor.matrix->get_line(yb);
				auto fileOffset = calculateFileOffset(descriptor, totalDimension, yb);

				//return MPI_File_read_at_all(
				return MPI_File_read_at(
					handle_, 
					fileOffset, 
					line, 
					descriptor.matrix->dimension(), 
					ConvertToMpiType <Matrix::value_type>::value, 
					MPI_STATUS_IGNORE
				);
			};

			for (int y = 0; y != descriptor.matrix->dimension(); ++y)
			{		
				int res = readLine(y);
				if (res != MPI_SUCCESS)
				{
					print_error(res);
					break;
				}
			}
		}

		/**
		 *	Write a block to the matrix.
		 */
		void writeBlock(BlockDescriptor descriptor, int totalDimension)
		{
			auto writeLine = [this, &descriptor, &totalDimension](int yb) -> int {
				auto* line = descriptor.matrix->get_line(yb);
				auto fileOffset = calculateFileOffset(descriptor, totalDimension, yb);

				//return MPI_File_write_at_all(
				return MPI_File_write_at(
					handle_, 
					fileOffset, 
					line, 
					descriptor.matrix->dimension(), 
					ConvertToMpiType <Matrix::value_type>::value, 
					MPI_STATUS_IGNORE
				);
			};

			for (int y = 0; y != descriptor.matrix->dimension(); ++y)
			{		
				int res = writeLine(y);
				if (res != MPI_SUCCESS)
				{
					print_error(res);
					break;
				}
			}
		}
		
		SharedMatrixFile& operator=(SharedMatrixFile const&) = delete;
		SharedMatrixFile& operator=(SharedMatrixFile&&) = delete;		

		SharedMatrixFile(SharedMatrixFile const&) = delete;
		SharedMatrixFile(SharedMatrixFile&&) = delete;

	private:
		long long calculateFileOffset(BlockDescriptor const& descriptor, int totalDimension, int yb);

	private:
		MPI_File handle_;
	};	
}
