#include "debug.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include "matrix/matrix.hpp"

#include <iostream>

//####################################################################################################################
void write_random_matrix(std::string const& name, int dimension, bool humanReadable)
{
    auto fileName = synthesize_file_name(matrix_default_location, name, dimension, humanReadable);
    Matrix result = generate_random_matrix(dimension);

    if (humanReadable)
        result.write_data(fileName);
    else
        result.write_binary(fileName);
}
//---------------------------------------------------------------------------------------------------------------------
void write_random_matrix(const std::string &name, int dimension)
{
    auto fileName = synthesize_file_name(matrix_default_location, name, dimension, false);
    auto fileNameH = synthesize_file_name(matrix_default_location, name, dimension, true);
    Matrix result = generate_random_matrix(dimension);
    result.write_binary(fileName);
    result.write_data(fileNameH);
}
//---------------------------------------------------------------------------------------------------------------------
void prepare_example(int dimension)
{
    auto fileNameA = synthesize_file_name(matrix_default_location, "A", dimension, false);
    auto fileNameAH = synthesize_file_name(matrix_default_location, "A", dimension, true);
    Matrix A = generate_random_matrix(dimension);
    A.write_binary(fileNameA);
    A.write_data(fileNameAH);

    auto fileNameB = synthesize_file_name(matrix_default_location, "B", dimension, false);
    auto fileNameBH = synthesize_file_name(matrix_default_location, "B", dimension, true);
    Matrix B = generate_random_matrix(dimension);
    B.write_binary(fileNameB);
    B.write_data(fileNameBH);

    (A*B).write_data(synthesize_file_name(matrix_default_location, "C", dimension, true));
}
//####################################################################################################################
