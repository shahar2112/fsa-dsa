/**************************************/
/* author: Shahar maimon              */
/* date: 02.27.20                     */
/**************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* fprintf */

#include "vsa.h"

#define WORD_SIZE (sizeof(size_t))
#define HEADER_SIZE (sizeof(header_t))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct header
{
   size_t block_size; /*the size that is remained to allocate */
}header_t;


struct vsa
{
   size_t pool_size;
}; /* vsa_t */

static size_t GetActualLen(header_t *header);
static void MarkAsOccupied(header_t *header);
static void MarkAsFreed(header_t *header);
static header_t *NextBlock(header_t *header);
static int IsBlockFree(header_t *header);
static void CreateHeader(header_t *header, size_t block_size);

/*Initialize the pool according to pool_size. (complexity: )*/
/******************** init ************************/
vsa_t *VSAInit(void *pool, size_t pool_size)
{
   vsa_t *vsa_handler = NULL;
   header_t *first_header = NULL;

   vsa_handler = pool;

   /* aligning pool address */
   while((size_t)vsa_handler%WORD_SIZE != 0)
   {
      (char *)vsa_handler++;
      pool_size--;
   }

   /* aligning end of pool to devide by two and show as freed 
   if last bit =0 so block is free*/
   if(pool_size%2 != 0)
   {
      pool_size--;
   }
   /*we can use last block in the struct instead of pool size */
   vsa_handler->pool_size = pool_size -sizeof(vsa_t);

   /* pointer to the header after handler */
<<<<<<< HEAD
   first_header = (header_t*)((unsigned char *)vsa_handler + sizeof(vsa_t));
   first_header->block_size = vsa_handler->pool_size - HEADER_SIZE;/*not vsa size???*/
=======
   first_header = (header_t*)((char *)vsa_handler + sizeof(vsa_t));
   first_header->block_size = vsa_handler->pool_size - HEADER_SIZE;
>>>>>>> 3e2d7023877b405da4ea6d1efc5ea45c5175f7c8

   return vsa_handler;
}


/*Allocating a memory block in a specific size, according to "first-fit" policy*/
/*reaction time may differ according number of allocated blocks*/
/******************** alloc ************************/
void *VSAAlloc(vsa_t *pool, size_t block_size)
{
   void *ret_address = NULL;
   header_t *header = NULL;
   void *end_of_pool = NULL; 

   assert(pool);
   assert(block_size <= pool->pool_size);

   end_of_pool = (unsigned char *)pool + pool->pool_size + sizeof(vsa_t);

   /*aligning block size */
/*    while(block_size%WORD_SIZE != 0)
   {
      block_size++;
   }
 */
   block_size += (WORD_SIZE- block_size%WORD_SIZE)%WORD_SIZE;

  /* pointer to the begining of pool after stuct (to first header) */
  /*  header = (header_t*)((unsigned char *)pool + sizeof(vsa_t)); */
  header = (header_t*)(pool + 1);

<<<<<<< HEAD
  while(header != end_of_pool)
  {
           /* if the block is free and with enough space so check if
           there is enough space for next header, create and break */
       if((IsBlockFree(header)) && (block_size <= GetActualLen(header)))
       {
         /* ret_address = (unsigned char *)header + HEADER_SIZE; */
        ret_address = header + 1;
         if((GetActualLen(header) - block_size) >=  (WORD_SIZE + HEADER_SIZE))
         {
               header_t* new_header = (header_t* )((unsigned char *)ret_address + block_size);
               new_header->block_size = (GetActualLen(header) -block_size - HEADER_SIZE);    
               header->block_size = block_size;
         }
         MarkAsOccupied(header);
         break;
       }
          /*if the current block free but not enough space
           so check if next block is free */
       else if((block_size > header->block_size) && IsBlockFree(header))
       {
=======
   /* while allocation data isn't even (occupied) so continue to the next block*/
   /* and if there is enough space to allocate block */
  while((void*)header < end_of_pool)
  {
      if((IsBlockFree(header)) && (block_size <= GetActualLen(header)))
      {
        /* ret_address = (unsigned char *)header + HEADER_SIZE; */
        ret_address = header + 1;

      CreateHeader(header, block_size);

        MarkAsOccupied(header);
        break;
      }
      else if((block_size > header->block_size) && IsBlockFree(header))
      {
         /*if the current block free but not enough space
         so check if next block is free */
>>>>>>> 3e2d7023877b405da4ea6d1efc5ea45c5175f7c8
         if(IsBlockFree(NextBlock(header)))
         {
            header->block_size += NextBlock(header)->block_size + HEADER_SIZE;
            continue;
         }
      }
               
        /* while allocation data isn't even (occupied)
          or free but not enough space
         so continue to the next block*/
       header = NextBlock(header);
  }
   return ret_address;
}



/*Frees a chosen block*/
/*Note: do not free a block that has already been freed. (complexity: O(1))*/
/******************** free ************************/
void VSAFree(void *block_to_free)
{
   header_t *header = NULL;

   assert(block_to_free);

   header = (header_t *)((unsigned char *)block_to_free - HEADER_SIZE);
   
   MarkAsFreed(header);
}




/*returns the size of the biggest free "block"*/
/******************** maxfreeblock ************************/
size_t MaxFreeBlockSize(vsa_t *pool)
{
   header_t *header = NULL;
   void* end_of_pool = NULL; 
   size_t max_free_block_size = 0;

   header = (header_t *)(pool + 1);
   end_of_pool = (unsigned char *)header + pool->pool_size;

   while ((void *)header < end_of_pool)
   {  
      /* if block is free */
      if(IsBlockFree(header))
      {
         max_free_block_size = MAX(max_free_block_size, GetActualLen(header));
      }
      
      header = NextBlock(header);
   }

   max_free_block_size -= (max_free_block_size % 8);

  return max_free_block_size;
}




/******************** more funcs ************************/
/******************** getreallen ************************/
static size_t GetActualLen(header_t *header)
{
   return header->block_size & ~1UL;
}

/******************** Markoccupied ************************/
static void MarkAsOccupied(header_t *header)
{
   header->block_size |= 1UL;
}

/*********************** MarkFree ************************/
static void MarkAsFreed(header_t *header)
{
   header->block_size &= ~1UL;
}

/*********************** NextBlock ************************/
static header_t *NextBlock(header_t *header)
{
   header_t *next_header = (header_t *)((char *)header + GetActualLen(header) + HEADER_SIZE);

   return next_header;
}

/*********************** IsBlockFree ************************/
static int IsBlockFree(header_t *header)
{
   return (header->block_size == (header->block_size & ~1ul));
  /*  return header->block_size %2 == 0; */
}
<<<<<<< HEAD
=======


/*********************** CreateNewHeader ************************/
static void CreateHeader(header_t *header, size_t block_size)
{
   void * ret_address = NULL;
   ret_address = header + 1;

         /* if the block is free and with enough space so create new header break */
      if((GetActualLen(header) - block_size) >=  (WORD_SIZE + HEADER_SIZE))
      {
         header_t* new_header = (header_t* )((unsigned char *)ret_address + block_size);
         new_header->block_size = (GetActualLen(header) -block_size - HEADER_SIZE);    
         header->block_size = block_size;
      }
}
>>>>>>> 3e2d7023877b405da4ea6d1efc5ea45c5175f7c8
