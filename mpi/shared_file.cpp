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
	SharedMatrixFile::~SharedMatrixFile()
	{
		MPI_File_close(&handle_);
	}	
//#####################################################################################################################
}
