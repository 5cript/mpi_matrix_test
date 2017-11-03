#include "matrix/matrix.hpp"
#include "bench/benchhelp.hpp"

#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
	//std::cout << benchWrap([]{Matrix A; A.read_data("/bigwork/nhmqebbe/data/A_1000x1000.data", 1000);}).count() << " ms\n";
	//std::cout << benchWrap([]{Matrix A; A.read_binary("/bigwork/nhmqebbe/data/A_1000x1000.bin", 1000);}).count() << " ms\n";
	
	Matrix A;
	if (A.read_binary("/bigwork/nhmqebbe/data/A_10x10.bin", 10))
	//if (A.read_data("/bigwork/nhmqebbe/data/A_10x10.data", 10))
	{
		if (A.write_binary("/bigwork/nhmqebbe/data/E_10x10.bin"));
		A.print();
	}
	
	return 0;
}
