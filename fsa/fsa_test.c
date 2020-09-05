#include <stdio.h>  /*printf */
#include <stdlib.h> /* malloc */

#include "fsa.h" /* forward declarations */
/*
#define TEST(real,expected) (real == expected ? printf("pass\n\n") : printf("fail\n\n"))

int main()
{
    fsa_t *pool = NULL;
    size_t block_size = 15;
    size_t pool_size = 100;
    size_t suggest = 0;
    void *element_address1 = NULL;
    void *element_address2 = NULL;
    void *element_address3 = NULL;

    pool = (fsa_t*)malloc(pool_size);
    if(NULL == pool)
    {
        fprintf(stderr, "Allocation failed");
        exit(1);
    }

    FSAInit(pool, pool_size, block_size);

   printf("FSAFreeCount = %ld\n", FSAFreeCount(pool)); 
   TEST(FSAFreeCount(pool), 4);
   
    element_address1 = FSAAlloc(pool);
    printf("FSAFreeCount after one alloc is = %ld\n", FSAFreeCount(pool)); 
   TEST(FSAFreeCount(pool), 3);
    element_address2 = FSAAlloc(pool);
    printf("FSAFreeCount after two allocs = %ld\n", FSAFreeCount(pool));
   TEST(FSAFreeCount(pool), 2);
    
    printf(("distance (bytes) between first and second alloc request (16):\n"));
    TEST((char *)element_address2 - (char *)element_address1, 16);

    printf(("the value of the new address after free and allocation is the same as element freed:\n"));
    FSAFree(pool, element_address2);
    element_address3 = FSAAlloc(pool);
    printf("element_address2: %p, element_address3: %p\n", element_address2, element_address3);
    TEST(element_address2 , element_address3);

    printf("FSAFreeCount before free is = %ld\n", FSAFreeCount(pool)); 
     FSAFree(pool, element_address2);
    printf("FSAFreeCount after one free is = %ld\n", FSAFreeCount(pool)); 
   TEST(FSAFreeCount(pool), 3);


    printf("the suggested size for 4 blocks of 15 is suppose to be (16*4 + 8*3 + 7 = 95) \n");
    suggest = FSASuggest(15, 4);
    printf("suggest is %ld\n", suggest);
    TEST(suggest, 95);

  free(pool);

    return 0;
}*/


/*
typedef struct person
{
    char *name;
    int age;
} person_t;

int main ()
{
    void *pool = NULL;
    fsa_t *handler = NULL;
    int numOfElements = 11;
    size_t suggesst = 0;
    person_t *p1 = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL;

    pool = malloc(207);
    if (NULL == pool)
    {
        fprintf(stderr, "allocation failed\n");
        exit (1);
    }
    printf("\t\tTESTING FSA\n\n");

    suggesst = FSASuggest(sizeof(person_t), numOfElements);
    printf("suggessted pool size is: %ld\n", suggesst);
    handler = FSAInit(pool, suggesst, sizeof(person_t));

    printf("asking for pointers to point at struct person. size of struct: %ld\n", sizeof(person_t));
    printf("passing a pool of size 207 bytes, and size of struct person\n");

    printf("freeCount is: %ld (expecting 11)\n\n", FSAFreeCount(handler));

    printf("allocating three person_t struct, containing char *name and int age\n\n");

    p1 = (person_t *) FSAAlloc(handler);
    printf("p1 address: %p, contains (before initializing): 0x%0lx\n", (void *)p1, *(size_t *)p1);
    p1->name = "Liad";
    p1->age = 31;
    printf("after initializing p1: %s, %d\n\n", p1->name, p1->age);

    p2 = (person_t *) FSAAlloc(handler);
    printf("p2 address: %p, contains (before initializing): 0x%0lx\n", (void *)p2, *(size_t *)p2);
    p2->name = "Tamar";
    p2->age = 28;
    printf("after initializing p2: %s, %d\n\n", p2->name, p2->age);

    p3 = (person_t *) FSAAlloc(handler);
    printf("p3 address: %p, contains (before initializing): 0x%0lx\n", (void *)p3, *(size_t *)p3);
    p3->name = "Gal";
    p3->age = 37;
    printf("after initializing p4: %s, %d\n\n", p3->name, p3->age);

    printf("freeCount is: %ld (expecting 8)\n", FSAFreeCount(handler));

    printf("freeing p3:\n");
    FSAFree(handler, p3);

    printf("freeCount is: %ld (expecting 9)\n\n", FSAFreeCount(handler));

    printf("allocating new person, expecting address of freed p3:\n");

    p4 = (person_t *) FSAAlloc(handler);
    printf("p4 address: %p, contains (before initializing): 0x%0lx\n", (void *)p4, *(size_t *)p4);
    p3->name = "Pumba";
    p3->age = 88;
    printf("after initializing p4: %s, %d\n\n", p4->name, p4->age);

    printf("freeCount is: %ld (expecting 8)\n\n", FSAFreeCount(handler));


    printf("freeing all allocations\n:");
    FSAFree(handler, p1);
    FSAFree(handler, p2);
    FSAFree(handler, p4);
    printf("freeCount is: %ld (expecting 11)\n\n", FSAFreeCount(handler));
   
    free(handler);
    return 0;
}
*/



#define TEST(real,expected) (real == expected ? printf("pass :)\n\n") : printf("fail :(\n\n"))

int main()
{
    void *my_pool;
    void *element_adress1, *element_adress2;
    fsa_t *pool_director;
    my_pool = (void *)malloc(100);
    pool_director = FSAInit(my_pool, 100, 15);

    printf ("\n\n100 bytes alloation. total number of available blocks is (4):\n");
	TEST(FSAFreeCount(pool_director),4);

    printf ("\n\ndistance (bytes) between first and second alloc request (16):\n");
    element_adress1 = FSAAlloc(pool_director);
    element_adress2 = FSAAlloc(pool_director);
    TEST((char *)element_adress2 - (char *)element_adress1,16);

    printf ("\n\nafter 2 allocs, number of free blocks is (2):\n");
    TEST(FSAFreeCount(pool_director),2);

    printf ("\n\ndoes the adress given to free equal to the adress got from next alloc? (YES!):\n");
    FSAFree(pool_director, element_adress2);
    TEST((element_adress2 == FSAAlloc(pool_director)),1);

    printf ("\n\nsuggest function for 4 elemtns, 15 bytes each (95, in bytes)\n\n");
    TEST((FSASuggest(15, 4) == 95),1);

    free(my_pool);

    printf ("\n\nnumber of elements after a new allocation according to the suggestion (4):\n\n");
    my_pool = (void *)malloc(95);
    pool_director = FSAInit(my_pool, 95, 15);
    TEST(FSAFreeCount(pool_director),4);

    free(my_pool);
    return 1;

    
    
}

/*



#define TEST(real,expected) (real == expected ? printf("%d, Pass\n", expected) : printf("%d, Fail\n",expected))

typedef struct fsa_test
{
    int x;
    int y;
    short z;
}test_t;


int main()
{
    void *data = NULL;
    fsa_t* Pool = NULL;
    test_t *TestVar1 = NULL;
    test_t *TestVar2 = NULL;

    data = malloc(88);
    if(NULL == data)
    {
        fprintf(stderr, "Allocation failed");
        exit(1);
    }

   /* printf("suggestion: %ld\n",FSASuggest(10,4));
    TEST(FSASuggest(10,4),95);
    Pool = FSAInit(data, 87, 10);
   /* printf("free count is - %ld\n", FSAFreeCount(Pool));
    TEST(FSAFreeCount(Pool),3);

    TestVar1 = (test_t *)FSAAlloc(Pool);
    TestVar1->x = 1;
    TestVar1->y = 2;
    TestVar1->z = 3;

   /* printf("the x data stored is - %d\n", TestVar1->x);*/
   /* TEST(TestVar1->x,1);*/
    /*printf("the y data stored is - %d\n", TestVar1->y);*/
 /*   TEST(TestVar1->y,2);

    TestVar2 = (test_t *)FSAAlloc(Pool);
    TestVar2->x = 4;
    TestVar2->y = 5;
    TestVar2->z = 6;*/

    /*printf("the x data stored is - %d\n", TestVar2->x);*/
   /* TEST(TestVar2->x,4);*/
   /* printf("free count is - %ld\n", FSAFreeCount(Pool));*/
  /*  TEST(FSAFreeCount(Pool),1);    
    FSAFree(Pool,TestVar1);*/

   /* printf("after free, the x data stored is - %d\n", TestVar1->x);*/

   /* printf("after free, free count is - %ld\n", FSAFreeCount(Pool));*/
  /*  TEST(FSAFreeCount(Pool),2);    

    free(data);
    data = NULL;
    return 0;
    
}*/
