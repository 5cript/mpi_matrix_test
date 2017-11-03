#include "matrix/matrix.hpp"
#include "bench/benchhelp.hpp"

#include <iostream>
#include <chrono>

#define BENCH(X) std::cout << benchWrap([&]{X}).count() << " ms\n"

int main(int argc, char** argv)
{
	//std::cout << benchWrap([]{Matrix A; A.read_data("/bigwork/nhmqebbe/data/A_1000x1000.data", 1000);}).count() << " ms\n";
	//std::cout << benchWrap([]{Matrix A; A.read_binary("/bigwork/nhmqebbe/data/A_1000x1000.bin", 1000);}).count() << " ms\n";
	
	Matrix A;
	Matrix B;
	A.read_binary("/bigwork/nhmqebbe/data/A_1000x1000.bin", 1000);
	B.read_binary("/bigwork/nhmqebbe/data/A_1000x1000.bin", 1000);

	//A.read_data("/bigwork/nhmqebbe/data2/A.data", 3);
	//B.read_data("/bigwork/nhmqebbe/data2/B.data", 3);


	Matrix C = A*B;

	BENCH(C.write_data("/bigwork/nhmqebbe/data/AA_1000x1000.data"););
	
	return 0;
}
