/*
 * sequencer.c
 *
 *  Created on: Mar 30, 2024
 *      Author: msonen
 */

#include "sequencer.h"
#include "smt.h"
#include "string.h"

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
		return SEQ_SUCCESS;
	return SEQ_RES_CORRUPT;
}


static void sequencer_node(char* str, Csmt<>& mt)
{
	   const char delim[2] = " ";
	   char *end_str;
	   char *token = strtok_r(str, delim, &end_str);
	   uint64_t key = strtoull(token, NULL, 10);
	   token = strtok_r(NULL, delim, &end_str);
	   mt.insert(key, std::string(token));
	   DBG("K: %ld V: %s\n", key, token);

}

seq_ret_t sequencer_publish(FILE* file, Csmt<>& mt)
{
	char *ptr = _buffer;
	size_t s = MAX_LEN_LENGTH;
	const char delim[2] = ",";
	char *end_str;

	if(-1 == getline(&ptr, &s, file))
		return SEQ_RES_CORRUPT;

	 char *token = strtok_r(ptr, delim, &end_str);

	 while(token != NULL)
	 {
		 sequencer_node(token, mt);
		 token = strtok_r(NULL, delim, &end_str);
	 }
	 //printf(_buffer);
	return SEQ_SUCCESS;

}


