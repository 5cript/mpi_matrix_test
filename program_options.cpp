#include "program_options.hpp"

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
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    

	if (vm.count("help")) {
	    std::cout << desc << "\n";
	    return boost::none;
	}

	return opts;
}
