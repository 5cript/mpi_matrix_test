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

enum class WriteStrategy
{
	/// Gather all data over network to the root process and write everything from there
	Gather = 0,

	/// Collectively write the data from all mpi instances.
	SharedFile = 1
};

struct ProgramOptions
{
	/// What dimension do the left and right matrix have? (square matrices)
	int dimension;

	/// What mpi rank shall be the core process that does the gathering?
	int root;

	/// Left matrix of the multiplication
	std::string leftMatrix;

	/// Right matrix of the multiplication.
	std::string rightMatrix;
	
	/// The matrix to write the result to.
	std::string resultMatrix;

	/// Are input matrices in human readable form or binary?
	bool humanReadableInput;

	/// Shall the output matrix be in human readable form or binary?
	bool humanReadableOutput; 

	/// UNUSED
	int idlePolicy;

	/// If set, will save the individual parts of the matrix as files on the filesystem.
	bool saveChunks;

	/// Determine whether or not chunks shall be in human readable format.
	bool chunksAreHumanReadable;

	/// The strategy to use for writing and gathering the result matrix.
	WriteStrategy writeStrategy;

	/// The directory where the chunks are stored, if enabled
	std::string partResultDirectory;
};

boost::optional <ProgramOptions> parse_arguments(int argc, char** argv);
