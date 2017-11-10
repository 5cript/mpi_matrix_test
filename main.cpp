#include "matrix/matrix.hpp"
#include "bench/benchhelp.hpp"
#include "mpi/mpi_core.hpp"
#include "mpi/communicator.hpp"

#include <iostream>
#include <chrono>

#include <thread>

static constexpr auto root = 0;

int main(int argc, char** argv)
{
	Mpi::Context ctx{root};
	Mpi::Communicator communicator{&ctx};
	std::cout << "id: " << ctx.id() << " size: " << ctx.size() << "\n";
	
	Matrix lhs;
	Matrix rhs;
	if (ctx.is_root())
	{
		// root must load the matrix from disk.
		lhs.read_binary("/bigwork/nhmqebbe/data/A_100x100.bin", 100);
		rhs.read_binary("/bigwork/nhmqebbe/data/B_100x100.bin", 100);
	}
	else
	{
		// those who are not root shall reserve space for this.
		lhs.resize(100);	
		rhs.resize(100);
	}

	// now spread the data from root to all other instances.
	communicator.broadcast <Matrix::value_type> (lhs.data(), lhs.data_size());
	communicator.broadcast <Matrix::value_type> (rhs.data(), rhs.data_size());
	
	return 0;
}
