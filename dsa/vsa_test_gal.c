#include<stdlib.h> /*malloc, exot*/
#include<stdio.h> /*fprintf*/

#include "vsa.h" /*inner library*/

/*MACRO TEST*/
#define TEST(real,expected) (real==expected)? printf("(%d), PASS\n", expected) : printf("(%d), FAIL\n", expected)

/*COLOR MACROS*/
#define RED printf("\033[1;31m");
#define DEF printf("\033[0m");
#define CYAN printf("\033[1;36m");
#define MAGENTA printf("\033[1;35m");

#define POOL_SIZE 100

static void *allocationCheck (void *ptr, char *name);

int main()
{
    vsa_t *vsa = NULL;
    
    int *p1 = NULL;
    int *p2 = NULL;
    int *p3 = NULL;
    int *p4 = NULL;
    int *nullPtr = NULL;

    size_t first_block_size = 10;
    size_t second_block_size = 15;
    size_t third_block_size = 20;
    size_t fourth_block_size = 5;

    vsa = (vsa_t *)malloc(POOL_SIZE);
    if (vsa==NULL)
    {
        fprintf(stderr, "ALLOCATION to pool failed\n");
        exit(1);
    }
    
    vsa = VSAInit(vsa, POOL_SIZE);
    RED;
    printf("Testing MaxFreeBlockSize after init:");
    TEST(MaxFreeBlockSize(vsa), 80);
    DEF;
    
    p1 = VSAAlloc(vsa, first_block_size);
    allocationCheck(p1, "p1");
    printf("success allocating p1!\n");
   
    RED;
    printf("\nTesting MaxFreeBlockSize after allocating p1:");
    TEST(MaxFreeBlockSize(vsa), 56);
    DEF;

    p2 = VSAAlloc(vsa, second_block_size);
    allocationCheck(p2, "p2"); 
    printf("success allocating p2!\n");
  
    MAGENTA;
    printf("Address of p2:\t");
    DEF;
    printf("%p\n", (void *)p2);
 
    RED;
    printf("\nTesting MaxFreeBlockSize after allocating p2:");
    TEST(MaxFreeBlockSize(vsa), 32);
    DEF;
    
    p3 = VSAAlloc(vsa, third_block_size);
    allocationCheck(p3, "p3"); 
    printf("success allocating p3!\n");

    VSAFree(p2);

    printf("%ld\n",MaxFreeBlockSize(vsa));

    p4 = VSAAlloc(vsa, fourth_block_size);
    allocationCheck(p4, "p4"); 
    printf("success allocating p4!\n");
    
    MAGENTA;
    printf("Address of p4:\t");
    DEF;
    printf("%p\n", (void *)p4);
 
    RED;
    printf("\nTesting MaxFreeBlockSize after allocating p2:");
    TEST(MaxFreeBlockSize(vsa), 0);
    DEF; 
    printf("\n");
    
    /*trying to allocate another ptr once list is full*/
    nullPtr = VSAAlloc(vsa, fourth_block_size);
    allocationCheck(nullPtr, "nullPtr");
    printf("Address of nullPtr: %p\n", (void *)nullPtr);

    free(vsa);
    return 0;
}

static void *allocationCheck (void *ptr, char *name)
{
    if (NULL == ptr)
    {
        fprintf(stderr, "FAILED TO ALLOCATE %s\n", name);
        exit(1);
    }
    return ptr;
}
