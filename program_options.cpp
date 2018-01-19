#include "program_options.hpp"
#include "helpers.hpp"

#include <boost/version.hpp>
#include <boost/program_options.hpp>
#include <iostream>

std::ostream& operator<<(std::ostream& stream, WriteStrategy write_strategy)
{
	return stream << static_cast <int> (write_strategy);
}

std::istream& operator>>(std::istream& stream, WriteStrategy write_strategy)
{
	int wsInt = 0;
	stream >> wsInt;
	write_strategy = static_cast <WriteStrategy> (wsInt);
	return stream;
}

boost::optional <ProgramOptions> parse_arguments(int argc, char** argv)
{
	namespace po = boost::program_options;

	ProgramOptions opts;

	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help", "produce help message")
	    ("dimension,d", po::value<int>(&opts.dimension)->default_value(100), "dimension of the square matrix d*d")
	    ("root,i", po::value<int>(&opts.root)->default_value(0), "the root process id")
	    ("lhs,l", po::value(&opts.leftMatrix), "The path to the left matrix")
	    ("rhs,r", po::value(&opts.rightMatrix), "The path to the right matrix")
		("output,o", po::value(&opts.resultMatrix), "The output matrix filename to put the result into")
	    (
			"human-readable-input,h", 
			po::bool_switch(&opts.humanReadableInput)->default_value(false), 
			"add, if the matrices are in human readable format"
		)
		(
			"human-readable-output,y", 
			po::bool_switch(&opts.humanReadableOutput)->default_value(false), 
			"add, if the output matrix shall be in human readable format"
		)
        (	
			"idle-policy,p", 
			po::value(&opts.idlePolicy)->default_value(static_cast <int> (IdlePolicy::FullIdle)), 
			"NOT IMPLEMENTED"
		)
        (
			"save-chunks", 
			po::bool_switch(&opts.saveChunks)->default_value(false), 
			"Save individual matrix chunks as files."
		)
        (
			"human-format-chunks", 
			po::bool_switch(&opts.chunksAreHumanReadable)->default_value(false), 
			"Will save intermediary chunks in human readable format, if set and 'save-chunks' is set."
		)
		(
			"chunk-dump-dir", 
			po::value(&opts.partResultDirectory)->default_value("/bigwork/nhmqebbe/results"), 
			"The directory where intermediary chunks are stored. This setting is ignored, if 'save-chunks' is not set."	
		)
		(	
			"write-strategy",
			po::value(&opts.writeStrategy)->default_value(WriteStrategy::SharedFile),
			"The strategy to use to collect the result matrix. 0 = gather over net, 1 = collective file write"
		)
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// default matrix locations (for LUH cluster):
	if (opts.leftMatrix.empty())
        opts.leftMatrix = synthesize_file_name("/bigwork/nhmqebbe/data", "A", opts.dimension, opts.humanReadableInput);
    if (opts.rightMatrix.empty())
        opts.rightMatrix = synthesize_file_name("/bigwork/nhmqebbe/data", "B", opts.dimension, opts.humanReadableInput);
	if (opts.resultMatrix.empty())
		opts.resultMatrix = synthesize_file_name("/bigwork/nhmqebbe/data", "M", opts.dimension, opts.humanReadableOutput);

	if (vm.count("help"))
    {
	    std::cout << desc << "\n";
	    return boost::none;
	}

	return opts;
}
