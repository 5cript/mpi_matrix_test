#include "block_list.hpp"

#include <iostream>
#include <fstream>

//####################################################################################################################
BlockList::BlockList(
	std::vector <Matrix::value_type>* blocks, 
	int blockDimension, 
	int overallDimension
)
	: blocks_{blocks}
	, blockDimension_{blockDimension}
	, overallDimension_{overallDimension}
{
}
//--------------------------------------------------------------------------------------------------------------------
void BlockList::print_block(int x, int y)
{
	auto blockDataSize = blockDimension_*blockDimension_;
	auto offset = (y * (overallDimension_ / blockDimension_) + x) * blockDataSize;
	std::cout << "Offset: " << offset << "\n";
	
	for (int i = 0; i != blockDimension_; ++i)
	{
		for (int j = 0; j != blockDimension_; ++j)
		{
			std::cout << (*blocks_)[offset + i * blockDimension_ + j] << ",";
		}
		std::cout << "\n";
	}
}
//--------------------------------------------------------------------------------------------------------------------
bool BlockList::save_matrix(std::string const& filename, bool humanReadable)
{
	//auto blockDataSize = blockDimension_*blockDimension_;
	auto blocksPerRow = overallDimension_ / blockDimension_;

	std::ofstream writer{filename, std::ios_base::binary};
	if (!writer.good())
		return false;

			
	for (int y = 0; y != overallDimension_; ++y)
	{
		for (int x = 0; x != overallDimension_; ++x)
		{
			auto index = 
				((y / blockDimension_) * blocksPerRow + x / blockDimension_) * blockDimension_ * blockDimension_ + 
				(x % blockDimension_ + (y % blockDimension_) * blockDimension_)
			;

			if (humanReadable)
			{
				writer << (*blocks_)[index];
				//writer << (*blocks_)[y * overallDimension_ + x];
				writer << ";";
			}
			else
				writer.write(reinterpret_cast <char const*> (&(*blocks_)[index]), sizeof(Matrix::value_type));
		}
		writer.put('\n');
	}
	return true;
}
//--------------------------------------------------------------------------------------------------------------------
bool BlockList::save_block_sequence(std::string const& filename)
{
	std::ofstream writer{filename, std::ios_base::binary};
	if (!writer.good())
		return false;

	for (std::size_t i = 0; i != blocks_->size(); ++i)
	{
		if (i%blockDimension_ == 0)
			writer << "\n";
		if (i%(blockDimension_*blockDimension_) == 0)
			writer << "\n";
		writer << (*blocks_)[i];
		writer << ",";
	}
	return true;
}
//####################################################################################################################
