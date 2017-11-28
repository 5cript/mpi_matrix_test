#pragma once

#include <string>

/**
 *	Creates a fileName (incl. path) for the presented naming convention of binary matrix files.
 *
 *	@param basePath Directory with matrices.
 *	@param matrixName Name of Matrix, eg "A"
 *	@param dimension The Dimension of the matrix.
 */
std::string synthesize_file_name(
    std::string basePath,
    std::string matrixName,
    int dimension,
    bool humanReadable
);
