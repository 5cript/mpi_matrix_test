#include "shared_file.hpp"
#include "error.h"

namespace Mpi
{
//#####################################################################################################################
    FileView make_matrix_file_view(int blockDimension)
    {
        FileView result;

        MPI_Datatype line;
        MPI_Type_contiguous(blockDimension, result.elementaryType, &line);
        result.filetype = line;

        return result;
    }
//#####################################################################################################################
	SharedMatrixFile::SharedMatrixFile(MPI_Comm communicator, std::string const& filename, FileStreamDirection openMode)
		: handle_{}
	{
		int mode = 0;
		if (openMode == FileStreamDirection::Read)
			mode = MPI_MODE_RDONLY;
		else
			mode = MPI_MODE_WRONLY | MPI_MODE_CREATE;

        auto res = MPI_File_open(communicator, filename.c_str(), mode, MPI_INFO_NULL, &handle_);
        if (res != 0)
        {
            print_error(res);
            std::cout << "File was: " << filename;
        }
	}
//---------------------------------------------------------------------------------------------------------------------
    void SharedMatrixFile::imbue_view(FileView const& view)
    {
        MPI_File_set_view(handle_, view.displacement, view.elementaryType, view.filetype, "native", MPI_INFO_NULL);
    }
//---------------------------------------------------------------------------------------------------------------------
	SharedMatrixFile::~SharedMatrixFile()
	{
		MPI_File_close(&handle_);
	}	
//#####################################################################################################################
}
