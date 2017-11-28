#include "program_options.hpp"
#include "helpers.hpp"

#include <boost/version.hpp>
#include <boost/program_options.hpp>
#include <iostream>

boost::optional <ProgramOptions> parse_arguments(int argc, char** argv)
{
	namespace po = boost::program_options;

	ProgramOptions opts;

	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help", "produce help message")
	    ("dimension,d", po::value<int>(&opts.dimension)->default_value(100), "dimension of the square matrix d*d")
	    ("root,i", po::value<int>(&opts.root)->default_value(0), "the root process id")
	    ("lhs,l", po::value(&opts.leftMatrix))
	    ("rhs,r", po::value(&opts.rightMatrix))
	    ("human-readable-input,h", po::bool_switch(&opts.humanReadableInput)->default_value(false))
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// default matrix locations (for LUH cluster):
	if (opts.leftMatrix.empty())
        opts.leftMatrix = synthesize_file_name("/bigwork/nhmqebbe/data", "A", opts.dimension, opts.humanReadableInput);
    if (opts.rightMatrix.empty())
        opts.rightMatrix = synthesize_file_name("/bigwork/nhmqebbe/data", "B", opts.dimension, opts.humanReadableInput);

	if (vm.count("help"))
    {
	    std::cout << desc << "\n";
	    return boost::none;
	}

	return opts;
}
