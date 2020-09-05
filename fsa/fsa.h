#ifndef __FSA_H__ 
#define __FSA_H__

#include <stddef.h> /* for size_t */

typedef struct pool_handler fsa_t; 

/* initializing received pool */
fsa_t *FSAInit(void *pool, size_t pool_size, size_t block_size);

/*  - O(1)*/
void *FSAAlloc(fsa_t *pool);

/* - O(1)*/
void FSAFree(fsa_t *pool, void *block_to_free);

/* returns size of pool to support block size * num_of_blocks */
size_t FSASuggest(size_t block_size, size_t num_of_blocks);

/* number of free blocks */
size_t FSAFreeCount(fsa_t *pool);

#endif	/* #ifndef __FSA_H__  */
