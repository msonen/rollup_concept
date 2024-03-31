/*
 * sequencer.c
 *
 *  Created on: Mar 30, 2024
 *      Author: msonen
 */

#include "sequencer.h"
#include "smt.h"
#include "block.h"
#include "string.h"
#include <unordered_map>


static char _buffer[MAX_LEN_LENGTH];

seq_ret_t sequencer_init(const char* seq_file, FILE** file)
{
	   FILE* fptr = fopen(seq_file, "r");

	   if(fptr == NULL)
		   	  return SEQ_INIT_FAIL;
	   *file = fptr;
	   return SEQ_SUCCESS;
}


seq_ret_t sequencer_close(FILE* file)
{
	if(fclose(file))
		return SEQ_RES_CORRUPT;
	return SEQ_SUCCESS;
}


static void sequencer_node(char* str, block* b)
{
	   const char delim[2] = " ";
	   char *end_str;
	   char *token = strtok_r(str, delim, &end_str);
	   uint64_t key = strtoull(token, NULL, 10);
	   token = strtok_r(NULL, delim, &end_str);
	   std::string value(token);
	   b->mt.insert(key, value);
	   b->data[key] = value;
	   DBG("K: %ld V: %s\n", key, value);

}

seq_ret_t sequencer_publish(FILE* file, block* curr_block)
{
	char *ptr = _buffer;
	size_t s = MAX_LEN_LENGTH;
	const char delim[2] = ",";
	char *end_str;
	ssize_t chars = getline(&ptr, &s, file);
	if(-1 == chars )
		return SEQ_RES_CORRUPT;

	if (ptr[chars - 1] == '\n') //remove newline
	    ptr[chars - 1] = '\0';

	 char *token = strtok_r(ptr, delim, &end_str);

	 while(token != NULL)
	 {
		 sequencer_node(token, curr_block);
		 token = strtok_r(NULL, delim, &end_str);
	 }
	return SEQ_SUCCESS;

}


