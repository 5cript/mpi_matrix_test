#include "parallel_matrix.hpp"

#include "matrix/matrix.hpp"
#include "matrix/matrix_partition.hpp"
#include "bench/benchhelp.hpp"
#include "mpi/communicator.hpp"
#include "helpers.hpp"
#include "parallel_matrix.hpp"

#include <iostream>

//#define DO_TRANSMIT

int parallelMul(Mpi::Context& ctx, int dimension)
{
	Mpi::Communicator communicator{&ctx};

	if (!ctx.is_root())
		return 0;
	
	Matrix lhs;
	Matrix rhs;
	if (ctx.is_root())
	{
		// root must load the matrix from disk.
		//if (!lhs.read_binary(synthesize_file_name("/bigwork/nhmqebbe/data", "A", dimension), dimension))
		if(!lhs.read_data("/bigwork/nhmqebbe/data/A_4x4.data", dimension))
		{
			std::cerr << "matrix does not exist: " << synthesize_file_name("/bigwork/nhmqebbe/data", "A", dimension) << "\n";
			return 2;
		}
		if(!rhs.read_data("/bigwork/nhmqebbe/data/B_4x4.data", dimension))
		//if (!rhs.read_binary(synthesize_file_name("/bigwork/nhmqebbe/data", "B", dimension), dimension))
		{
			std::cerr << "matrix does not exist: " << synthesize_file_name("/bigwork/nhmqebbe/data", "B", dimension) << "\n";
			return 2;
		}
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
	/*	
	int div = ctx.size();
	for (; div < dimension / 2; ++div)
		if (dimension % div == 0)
			break;
	if (dimension % div != 0)
	{
		std::cerr << "cannot partition matrix.\n";
		return 1;
	}
	*/
	int div = 2;

	Matrix resultMatrix{dimension};
	auto resultPartition = MatrixPartition{&resultMatrix, div};
	auto leftPartition = MatrixPartition{&lhs, div};
	auto rightPartition = MatrixPartition{&rhs, div};

	std::cout << div << "\n";

	/*	
	for (int Cy = 0; Cy != div; ++Cy)
		for (int Cx = 0; Cx != div; ++Cx) 
			for (int i = 0; i != div; ++i)
			{
				leftPartition.aquire(i, Cy).accum_multiply(
					rightPartition.aquire(Cx, i), resultPartition.aquire(Cx, Cy)
				);
			}
	*/

	for (int y = 0; y != div; ++y)
	{
		for (int x = 0; x != div; ++x)
		{
			Matrix temp{div};
			for (int i = 0; i != div; ++i)
			{
				//std::cout << "B" << y+1 << i+1 << "*" << "C" << i+1 << x+1 << "+";
				leftPartition.aquire(x, i).print(); std::cout << "*\n";
				rightPartition.aquire(i, y).print(); std::cout << "+\n";
				temp = temp + leftPartition.aquire(y, i).clone() * rightPartition.aquire(i, x).clone();
			}
			std::cout << "=";
			temp.print();

			for (int i = 0; i != div; ++i)
				resultPartition.aquire(x, y) = resultPartition.aquire(x, y).clone() +
					(leftPartition.aquire(y, i).clone() * rightPartition.aquire(i, x).clone());

			// TODO: Improveable
/*
			value_type sum = 0;
			for (int i = 0; i != dimension_; ++i)
				sum += data_[COORD(i, y)] * other.data_[COORD(x, i)];
			result.data_[COORD(x, y)] = sum;
*/
		}
			std::cout << "\n";
	}


	if (ctx.is_root())
		resultMatrix.print(100);

	std::cout << "\n";

	(lhs*rhs).print();
	return 0;
}
