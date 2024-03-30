/*
 * block.h
 *
 *  Created on: Mar 31, 2024
 *      Author: msonen
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#define DATA_MAX_LEN	64
#define	DATA_MAX		256

#include <unordered_map>

enum FinalizationStatus
{
	Trusted,
	DaNotFinalized,
	DaFinalized
};

typedef struct
{
	std::unordered_map<uint64_t, std::string> data;
	FinalizationStatus status;

}block_t;


void block_init(block_t* block);

#endif /* BLOCK_H_ */
