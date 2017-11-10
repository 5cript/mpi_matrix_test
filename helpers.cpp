#include "helpers.hpp"

#include <sstream>

std::string synthesize_file_name(std::string basePath, std::string matrixName, int dimension)
{
	std::stringstream sstr;
	sstr << basePath
	     << "/"	
	     << matrixName
	     << "_"
	     << dimension
	     << "x"
	     << dimension
	     << ".bin"	
	;
	return sstr.str();
}
