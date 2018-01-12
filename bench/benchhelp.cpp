#include "benchhelp.hpp"

#include <stdexcept>
#include <iostream>
#include <algorithm>

//####################################################################################################################
TimeStampCollection::TimeStampCollection()
	: constructionTime_{std::chrono::high_resolution_clock::now()}
{
}
//--------------------------------------------------------------------------------------------------------------------
void TimeStampCollection::add(std::string const& name)
{
	auto now = std::chrono::high_resolution_clock::now();
	stamps_.emplace_back(
		name,
		std::chrono::duration_cast <std::chrono::milliseconds> (now - constructionTime_)
	);
}
//--------------------------------------------------------------------------------------------------------------------
void TimeStampCollection::dump()
{
	for (auto const& i : stamps_)
		std::cout << i.first << ": " << i.second.count() << " ms\n";
}
//####################################################################################################################
std::chrono::milliseconds benchWrap(std::function <void()> func)
{
	auto now = std::chrono::high_resolution_clock::now();
	func();
	return std::chrono::duration_cast <std::chrono::milliseconds> (std::chrono::high_resolution_clock::now() - now);
}	
//####################################################################################################################
