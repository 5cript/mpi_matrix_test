#include "benchhelp.hpp"

std::chrono::milliseconds benchWrap(std::function <void()> func)
{
	auto now = std::chrono::high_resolution_clock::now();
	func();
	return std::chrono::duration_cast <std::chrono::milliseconds> (std::chrono::high_resolution_clock::now() - now);
}	
