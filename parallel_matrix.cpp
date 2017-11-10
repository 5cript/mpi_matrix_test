#include "parallel_matrix.hpp"

#include "matrix/matrix.hpp"
#include "bench/benchhelp.hpp"
#include "mpi/communicator.hpp"
#include "helpers.hpp"
#include "parallel_matrix.hpp"

#include <iostream>

int parallelMul(Mpi::Context& ctx, int dimension)
{
	Mpi::Communicator communicator{&ctx};
	
	Matrix lhs;
	Matrix rhs;
	if (ctx.is_root())
	{
		// root must load the matrix from disk.
		if (!lhs.read_binary(synthesize_file_name("/bigwork/nhmqebbe/data", "A", dimension), dimension))
			return 2;
		if (!rhs.read_binary(synthesize_file_name("/bigwork/nhmqebbe/data", "B", dimension), dimension))
			return 2;
	}
	else
	{
		// those who are not root shall reserve space for this.
		lhs.resize(dimension);	
		rhs.resize(dimension);
	}	

#ifdef DO_TRANSMIT
	// now spread the data from root to all other instances.
	communicator.broadcast <Matrix::value_type> (lhs.data(), lhs.data_size());
	communicator.broadcast <Matrix::value_type> (rhs.data(), rhs.data_size());
#endif // DO_TRANSMIT

	// determine the block width for the partitions (the matrix dimension is never prime)
	int div = ctx.size();
	for (; div != dimension / 2; ++div)
		if (dimension % div == 0)
			break;

	if (dimension % div != 0)
	{
		std::cerr << "cannot partition matrix.\n";
		return 1;
	}

	std::cout << dimension << "\n";
	return 0;
}
