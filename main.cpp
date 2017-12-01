#include "mpi/mpi_core.hpp"
#include "program_options.hpp"
#include "parallel_matrix.hpp"

#include <iostream>
#include <chrono>
#include <iterator>
#include <vector>
#include <thread>

template<template <typename...> class ContainerT, typename T>
std::ostream& operator<<(std::ostream& s, ContainerT<T> t) { 
    if (t.empty())
    {
        s << "[]";
        return s;
    }
    s << '[';
    auto i = std::begin(t);
    for (; i != std::end(t); ++i)
    {
        auto j = i;
        ++j;
        if (j == std::end(t))
            break;
        s << *i << ',';
    }
    ++i;
    s << *i;
    return s << ']' << std::endl;
}

int main(int argc, char** argv)
{
	// Parameters
	auto options = parse_arguments(argc, argv);
	if (!options)
		return 1;

    // The MPI context handles everything MPI world related.
	Mpi::Context ctx{options.get().root};   
    if (ctx.is_root())
    {
        auto optimalInstances = optimal_instances(options.get().dimension);

        std::cout << optimalInstances << "\n\n";
    
        if (optimalInstances.find(ctx.size()) == optimalInstances.end())
            std::cout << "WARNING: The amount of MPI instances is not optimal. These are theoretical optimal values \n"
                      << optimalInstances << "\n";
        else
            std::cout << "MPI instances optimal, no idling will happen: " << ctx.size() << "\n";
    }

	// Actual Job
    parallel_multiplication(ctx, options.get());

	return 0;
}
