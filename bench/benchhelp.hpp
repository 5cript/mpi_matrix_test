#pragma once

#include <chrono>
#include <functional>
#include <vector>
#include <string>

/// A collection of important time stamps for calculation of certain key values.
class TimeStampCollection
{
public:
	using time_unit = std::chrono::milliseconds;

	/**
 	 *	
	 */
	TimeStampCollection();

	/** 
	 *	Add a timestamp for the current time. They are relative to the construction of this class.
	 */
	void add(std::string const& name);
	
	/**
 	 *	Calculate a time difference between two different time stamps.
 	 */
	//time_unit difference(std::string const& older, std::string const& newer);

	/**
 	 *	Dump all stamps to stdout.
	 */
	void dump();

private:
	std::chrono::high_resolution_clock::time_point constructionTime_;
	std::vector <std::pair <std::string, time_unit> > stamps_;
};

/**
 *	Runs a benchmark on a given function.
 */
std::chrono::milliseconds benchWrap(std::function <void()> func);

#define BENCH(X) std::cout << benchWrap([&]{X}).count() << " ms\n"
