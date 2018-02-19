#pragma once

#include <string>

/**
 * @brief generates a matrix at the default location specified in constants.hpp and auto names it
 * @param dimension The DxD dimension.
 */
void write_random_matrix(const std::string &name, int dimension, bool humanReadable);
void write_random_matrix(const std::string &name, int dimension);
void prepare_example(int dimension);
