#include <stdio.h>
#include <stdlib.h>

#include "vsa.h"

#define TEST(real,expected) (real == expected ? printf("%d, Pass\n", expected) : printf("%d, Fail\n",expected));

#define RED	printf("\033[1;31m")
#define BLUE printf("\033[1;36m")
#define MAGENTA printf("\033[1;35m")
#define YELLOW printf("\033[1;33m")
#define DEFAULT	printf("\033[0m")

typedef struct person
{
    char *name;
    int age;
} person_t;

static void PrintPerson(person_t *person);


int main()
{
    void *pool = NULL;
    vsa_t *pool_handler = NULL;
    size_t pool_size = 100;
    size_t max_free = 0;
    person_t *p1 = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL;

    pool = malloc(100);
    if (NULL == pool)
    {
        fprintf(stderr, "allocation failed\n");
        exit (1);
    }

    pool_handler = VSAInit(pool, pool_size);

    RED;
    max_free = MaxFreeBlockSize(pool_handler);
    printf("max free block size (expected 100 - 8 - 8-4(alignment)) \n, %ld ", max_free);
    TEST(max_free , 80);
    DEFAULT;


    BLUE;
    p1 = VSAAlloc(pool_handler, sizeof(person_t));

    max_free = MaxFreeBlockSize(pool_handler);
    printf("max free block size (expected 56) ,  maxfree is %ld\n", max_free);
    TEST(max_free , 56);
    DEFAULT;
    
    p1->age = 20;
    p1->name = "Tal";

    PrintPerson(p1);

    YELLOW;
    /* allocating another person */
    p2 = (person_t *)VSAAlloc(pool_handler, sizeof(person_t));
    if (NULL == p2)
    {
        printf("allocation failed\n");
    }

    max_free = MaxFreeBlockSize(pool_handler);
    printf("max free block size after second allocation is (expected 60 -%d -8 -alignment) ,  maxfree is %d\n", sizeof(person_t), max_free);
    TEST(max_free , 32);
    DEFAULT;


 MAGENTA;
    printf("testing allocating more than free space\n");
    p2 = (person_t *)VSAAlloc(pool_handler, 84);
    if (NULL == p2)
    {
        printf("allocation failed\n");
        
    } 

    max_free = MaxFreeBlockSize(pool_handler);
    printf("max free block size (expected 84 - %d) ,  maxfree is %d\n", sizeof(person_t), max_free);
    TEST(max_free , 0);
DEFAULT;

VSAFree(p1);
VSAFree(p2);

free(pool);
pool = NULL;

    return 0;
}

static void PrintPerson(person_t *person)
{
    printf("name: %s\t", person->name);
    printf("age: %d\n", person->age);
}