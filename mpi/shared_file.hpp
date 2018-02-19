	#pragma once

#include "mpi_core.hpp"
#include "communicator.hpp"
#include "types.hpp"

#include "../matrix/block_descriptor.hpp"
#include "../matrix/matrix_value_type.hpp"

namespace Mpi
{
	enum class FileStreamDirection
	{
		Read,
		Write
	};

    struct FileView
    {
        int displacement{0};
        MPI_Datatype elementaryType{ConvertToMpiType <matrix_value_type>::value};
        MPI_Datatype filetype;
    };

    FileView make_matrix_file_view(int blockDimension);

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
		template <typename MatrixViewT>
        void readBlock(BlockDescriptor <MatrixViewT> descriptor, int totalDimension)
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
					ConvertToMpiType <matrix_value_type>::value, 
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
		template <typename MatrixViewT>
        void writeBlock(BlockDescriptor <MatrixViewT> descriptor, int totalDimension, bool at_all)
		{
            auto writeLine = [this, &descriptor, &totalDimension, &at_all](int yb) -> int {
				auto* line = descriptor.matrix->get_line(yb);
				auto fileOffset = calculateFileOffset(descriptor, totalDimension, yb);

                auto* func = MPI_File_write_at;
                if (at_all)
                    func = MPI_File_write_at_all;

                return func(
					handle_, 
					fileOffset, 
					line, 
					descriptor.matrix->dimension(), 
					ConvertToMpiType <matrix_value_type>::value, 
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

        void imbue_view(FileView const& view);
		
		SharedMatrixFile& operator=(SharedMatrixFile const&) = delete;
		SharedMatrixFile& operator=(SharedMatrixFile&&) = delete;		

		SharedMatrixFile(SharedMatrixFile const&) = delete;
		SharedMatrixFile(SharedMatrixFile&&) = delete;

	private:
		template <typename MatrixViewT>
		long long calculateFileOffset(BlockDescriptor <MatrixViewT> const& descriptor, int totalDimension, int yb)
		{
			long long fileOffset = 
				descriptor.x * descriptor.matrix->dimension() + 
				descriptor.y * descriptor.matrix->dimension() * totalDimension +				 
				yb * totalDimension		 
			;	

			fileOffset *= sizeof(matrix_value_type);	
			return fileOffset;
		}

	private:
		MPI_File handle_;
	};	
}
