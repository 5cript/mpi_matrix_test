#pragma once

#include <boost/optional.hpp>

enum class IdlePolicy
{
    /// Some Instances will idle during the entire computation.
    FullIdle = 0,

    /// Distribute remaining chunks over to some instances.
    /// The other instances will idle while the remaining ones do work.
    Distribute = 1
};

struct ProgramOptions
{
	int dimension;
	int root;
    std::string leftMatrix;
    std::string rightMatrix;
    bool humanReadableInput;
    int idlePolicy;
};

boost::optional <ProgramOptions> parse_arguments(int argc, char** argv);
