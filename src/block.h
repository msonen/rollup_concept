/*
 * block.h
 *
 *  Created on: Mar 31, 2024
 *      Author: msonen
 */

#ifndef BLOCK_H_
#define BLOCK_H_


#include <unordered_map>
#include "smt.h"


enum FinalizationStatus
{
	Trusted,
	DaNotFinalized,
	DaFinalized
};

typedef struct
{
		std::unordered_map<uint64_t, std::string> data; // just for changes, not entire data
		Csmt<> mt;
		FinalizationStatus status;
}block;


void block_init(block* block);
void block_copy(block* block_to_be_cpoied, block* b);

#endif /* BLOCK_H_ */
