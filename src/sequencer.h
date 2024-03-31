/*
 * db.h
 *
 *  Created on: Mar 30, 2024
 *      Author: msonen
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "stdio.h"
#include "smt.h"
#include "block.h"
#include <unordered_map>

#define MAX_LEN_LENGTH	(size_t)8192

//#define __DEBUG_ENABLE__

#ifdef	__DEBUG_ENABLE__
#define DBG(format, ...)	printf(format, __VA_ARGS__)
#else
#define DBG(format, ...)
#endif

typedef enum
{
	SEQ_SUCCESS = 0,
	SEQ_INIT_FAIL,
	SEQ_RES_CORRUPT
}seq_ret_t;


seq_ret_t sequencer_init(const char* seq_file, FILE** file);
seq_ret_t sequencer_publish(FILE* file, block* curr_block);
seq_ret_t sequencer_close(FILE* file);



#endif /* SEQUENCER_H_ */
