#include "helpers.hpp"

#include <sstream>

std::string synthesize_file_name(
    std::string basePath,
    std::string matrixName,
    int dimension,
    bool humanReadable
)
{
	std::stringstream sstr;
	sstr << basePath
	     << "/"
	     << matrixName
	     << "_"
	     << dimension
	     << "x"
	     << dimension
    ;
    if (humanReadable)
        sstr << ".data";
    else
        sstr << ".bin";
	return sstr.str();
}
