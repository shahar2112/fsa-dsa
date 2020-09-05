/**************************************/
/* author: Shahar maimon              */
/* date: 02.22.20                     */
/**************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */


#include "fsa.h" /* forward declarations */

#define WORD_SIZE (8)
 
struct pool_handler
{
    void *next_free_address;
    size_t max_blocks;
    size_t free_count;
}; /* fsa_t */

/* initializing received pool */
/*******************************************************************/
fsa_t *FSAInit(void *pool, size_t pool_size, size_t block_size)
{
     size_t *start_address = NULL;
     size_t *end_address = NULL;
     fsa_t* pool_handler = NULL;

    unsigned int i = 0;

    pool_handler = pool; 

    /* aligning pool address */
/*     while((size_t)pool%WORD_SIZE != 0)
    {
        (unsigned char *)pool_handler++;
        pool_size--;
    } */

    if((size_t)pool%WORD_SIZE)
    {
        pool_handler += (WORD_SIZE - (size_t)pool%WORD_SIZE);
        pool_size -=  (WORD_SIZE - (size_t)pool%WORD_SIZE);
    }
    /* start address */
    start_address = (size_t*)((unsigned char *)pool + sizeof(fsa_t));

    /* aligning block size */
    while(block_size%WORD_SIZE != 0)
    {
        block_size++;
    }

   pool_handler->next_free_address = start_address;
   pool_handler->max_blocks = (pool_size-sizeof(fsa_t))/block_size;
   pool_handler->free_count = pool_handler->max_blocks;


    for(i=0; i<(pool_handler->free_count-1); i++)
    {
        *start_address = (size_t)((unsigned char *)start_address + block_size);
        start_address = start_address + block_size/WORD_SIZE;
    }

    /* last element should contain 0 */
     end_address = (size_t*)((unsigned char *)pool_handler + pool_size - block_size);
    *end_address = 0; 

   return pool_handler;
} 


/********************************************************************************************/
/* frees specific block */
void FSAFree(fsa_t *pool, void *block_to_free)
{
    void *tmp_address = NULL;
    
    assert(pool);
    assert(block_to_free);
    assert((pool->free_count) <= (pool->max_blocks)); /* you can't free more than max_blocks */

    tmp_address = block_to_free;

    /* next free address is now saved in the struct */
    *(size_t *)block_to_free = (size_t)pool->next_free_address;

    pool->next_free_address = tmp_address;

    /*one more free space */
    pool->free_count++;
}

/********************************************************************************************/
/* alllocate block */
void *FSAAlloc(fsa_t *pool)
{
    void *return_address = NULL;

    assert(pool);   
    assert (pool->free_count>0); /* you can't allocate when there is no more free space */ 

    /* the tmp_address is the value of next free address */
    return_address = pool->next_free_address;

    /* the value of tmp_address is the value of next free address */
    pool->next_free_address = (void*)(*(size_t*)return_address);

    /*one less free space */
    pool->free_count--;

    return return_address;
}


/********************************************************************************************/
/* returns size of pool to support block size * num_of_blocks */
size_t FSASuggest(size_t block_size, size_t num_of_blocks)
{
    size_t suggest_size = 0;

    while(block_size%WORD_SIZE != 0)
    {
        block_size++;
    }

    /* suggest size is the size of blocks(alligned)*number of blocks +
         size of struct +number of max allignment needed*/
    suggest_size = block_size*num_of_blocks + sizeof(fsa_t) + (WORD_SIZE-1);
        
    return suggest_size;
}


/********************************************************************************************/
/* number of free blocks */
size_t FSAFreeCount(fsa_t *pool)
{
    assert(pool);
    
    return pool->free_count;
}
