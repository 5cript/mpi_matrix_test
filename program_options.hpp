#pragma once

#include <boost/optional.hpp>

struct ProgramOptions
{
	int dimension;
	int root;
};

boost::optional <ProgramOptions> parse_arguments(int argc, char** argv); 
