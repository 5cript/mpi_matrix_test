#pragma once

#include <boost/optional.hpp>

struct ProgramOptions
{
	int dimension;
	int root;
    std::string leftMatrix;
    std::string rightMatrix;
    bool humanReadableInput;
};

boost::optional <ProgramOptions> parse_arguments(int argc, char** argv);
