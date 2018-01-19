#include "shared_file.hpp"

namespace Mpi
{
//#####################################################################################################################
	SharedMatrixFile::SharedMatrixFile(MPI_Comm communicator, std::string const& filename, FileStreamDirection openMode)
		: handle_{}
	{
		int mode = 0;
		if (openMode == FileStreamDirection::Read)
			mode = MPI_MODE_RDONLY;
		else
			mode = MPI_MODE_WRONLY | MPI_MODE_CREATE;

		MPI_File_open(communicator, filename.c_str(), mode, MPI_INFO_NULL, &handle_);
	}
//---------------------------------------------------------------------------------------------------------------------
	long long SharedMatrixFile::calculateFileOffset(BlockDescriptor const& descriptor, int totalDimension, int yb)
	{
		long long fileOffset = 
			descriptor.x * descriptor.matrix->dimension() + 
			descriptor.y * descriptor.matrix->dimension() * totalDimension +				 
			yb * totalDimension		 
		;	

		fileOffset *= sizeof(Matrix::value_type);	
		return fileOffset;
	}
//---------------------------------------------------------------------------------------------------------------------
	SharedMatrixFile::~SharedMatrixFile()
	{
		MPI_File_close(&handle_);
	}	
//#####################################################################################################################
}
