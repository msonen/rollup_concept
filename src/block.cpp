/*
 * block.c
 *
 *  Created on: Mar 31, 2024
 *      Author: msonen
 */

#include "block.h"


void block_init(block* block)
{
	block->status = Trusted;
}


//TODO make an efficient way
void block_copy(block* block_to_be_cpoied, block* b)
{
	block_to_be_cpoied->status = b->status;
	for(auto& key: b->data)
	{
		block_to_be_cpoied->mt.insert(key.first, key.second);
		block_to_be_cpoied->data[key.first] = key.second;
	}
}

