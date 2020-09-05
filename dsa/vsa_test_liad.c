#include <stdlib.h> /*malloc, free..*/
#include <stdio.h> /*fprintf,..*/

#include "vsa.h"

#define PERSON_SIZE sizeof(person_t)

typedef struct person
{
    size_t id;
    int age;
}person_t;

static void printPerson(person_t *person);
void AllocationCheck(void *pointerToCheck, char *pointerName);


int main()
{
    void *allocation = NULL;
    vsa_t *pool = NULL;
    size_t freeBlockSize = 0;
    person_t *p1, *p2, *p3;
    int *IntToAlloc = NULL;
    size_t *SizeTtoAlloc = NULL;
    void *ptrForBigSize = NULL;
    void *ptrForNULL = NULL;

    allocation = malloc(250);
    if (NULL == allocation)
    {
        fprintf(stderr, "allocation failed\n");
        exit (3);
    }

    pool = VSAInit(allocation, 250);
    /*testing MaxFreeBlockSize*/
    freeBlockSize = MaxFreeBlockSize(pool);
    printf("after init with size 250, expected max free block size is: 232\n");
    printf("max free block size is: %ld\n", freeBlockSize);
    printf("allocating for size %ld\n", PERSON_SIZE);

    p1 = (person_t *)VSAAlloc(pool, PERSON_SIZE);
    AllocationCheck((void *)p1, "p1");

    freeBlockSize = MaxFreeBlockSize(pool);
    printf("after allocating, expected max free block size is: 208\n");
    printf("max free block size is: %ld\n", freeBlockSize);

    p2 = (person_t *)VSAAlloc(pool, PERSON_SIZE);    
    AllocationCheck((void *)p2, "p2");
    p3 = (person_t *)VSAAlloc(pool, PERSON_SIZE);  
    AllocationCheck((void *)p3, "p3");

    printf("allocating 2 more, expected max free block size is: 160\n"); 
    freeBlockSize = MaxFreeBlockSize(pool);
    printf("max free block size is: %ld\n", freeBlockSize);   

    /*initializing p's*/
    p1->id = 123456789;
    p1->age = 30;
    p2->id = 111222333;
    p2->age = 31;
    p3->id = 555555555;
    p3->age = 32;

    /*making sure allocation worked - printing p's*/
    printf("testing allocation - printing person structs:\n");
    printPerson(p1);
    printPerson(p2);
    printPerson(p3);

    printf("allocating size of int:\n");    
    IntToAlloc = (int *)VSAAlloc(pool, sizeof(int));
    AllocationCheck((void *)IntToAlloc, "IntToAlloc");
    printf("expected max free block size is: 144\n"); 
    freeBlockSize = MaxFreeBlockSize(pool);
    printf("max free block size is: %ld\n", freeBlockSize);   
    *IntToAlloc = 5;
    printf("allocated int (expected 5): %d\n", *IntToAlloc);

    printf("testing free:\n");
    printf("freeing block of size 16 (including header: 24), in address: %p\n", (void *)p2);
    VSAFree(p2);

    printf("allocating new block of size 8. expecting same address as freed block\n");
    SizeTtoAlloc = (size_t *)VSAAlloc(pool, sizeof(size_t));
    AllocationCheck((void *)SizeTtoAlloc, "SizeTtoAlloc");
    *SizeTtoAlloc = 5;
    printf("allocated block address: %p\n", (void *)SizeTtoAlloc);
    printf("allocated block value (expecting 5): %lu\n", *SizeTtoAlloc);

    printf("expected max free block size is: 144\n"); 
    freeBlockSize = MaxFreeBlockSize(pool);
    printf("max free block size is: %ld\n", freeBlockSize);   

    printf("allocating max free block size\n");
    ptrForBigSize = VSAAlloc(pool, 136);
    AllocationCheck((void *)ptrForBigSize, "ptrForBigSize");
    printf("expected max free block size is: 0\n"); 
    freeBlockSize = MaxFreeBlockSize(pool);
    printf("max free block size is: %ld\n", freeBlockSize);  

    ptrForNULL = VSAAlloc(pool, 10);
    printf("allocating again, expecting allocation to fail\n");
    AllocationCheck((void *)ptrForNULL, "ptrForNULL");



    free(allocation);
    return 0;
}


static void printPerson(person_t *person)
{
    printf("id: %lu\t", person->id);
    printf("age: %u\n", person->age);
}

void AllocationCheck(void *pointerToCheck, char *pointerName)
{
    if (NULL == pointerToCheck)
    {
        printf("allocation for %s failed\n", pointerName);
    }
    else
    {
        printf("allocation for %s succeeded\n", pointerName);
    }
}
