#ifndef __VSA_H__ 
#define __VSA_H__

#include <stddef.h> /*size_t*/

typedef struct vsa vsa_t; 

/*Initialize the pool according to pool_size. (complexity: )*/
vsa_t *VSAInit(void *pool, size_t pool_size);

/*Allocating a memory block in a specific size, according to "first-fit" policy*/
/*reaction time may differ according number of allocated blocks*/
void *VSAAlloc(vsa_t *pool, size_t block_size);

/*Frees a chosen block*/
/*Note: do not free a block that has already been freed. (complexity: O(1))*/
void VSAFree(void *block_to_free);

/*returns the size of the biggest free "block"*/
size_t MaxFreeBlockSize(vsa_t *pool);

#endif	/* #ifndef __VSA_H__  */