#pragma once

#include <boost/optional.hpp>

struct ProgramOptions
{
	int dimension;
};

boost::optional <ProgramOptions> parse_arguments(int argc, char** argv); 
