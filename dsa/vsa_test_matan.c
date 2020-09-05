#include <stdio.h> /*printf*/
#include <stdlib.h> /* malloc */
#include "vsa.h"

#define TEST(real,expected) (real == expected ? printf("%d, Pass\n", expected) : printf("%d, Fail\n",expected))

typedef struct test12bytes
{
    int x;
    char *z;
}test16_allign;

typedef struct test5bytes
{
    int x;
    char c;
}test8_allign;

typedef struct test17bytes
{
    int x;
    int y;
    int *z;
    char c;
}test24_allign;

int main()
{  
    vsa_t *vsa = NULL;
    void *pool = NULL;
    size_t pool_size = 100;
    test16_allign *test_16 = NULL;
    test16_allign *test_16_another = NULL;
    test8_allign *test_8 = NULL;
    test8_allign *test_8_another = NULL;
    test24_allign *test_24 = NULL;
    int check = 8;

    pool = malloc(pool_size);
    if(pool == NULL)
	{
		fprintf(stderr, "Allocation failed");
		exit(1);
	} 

    vsa = VSAInit(pool, pool_size);
    TEST(MaxFreeBlockSize(vsa),80);

    test_16 = (test16_allign *)VSAAlloc(vsa, sizeof(test16_allign));
    test_16->x = 4;
    test_16->z = "abc";
    TEST(test_16->x,4);
    TEST(MaxFreeBlockSize(vsa),56);

    test_8 = (test8_allign *)VSAAlloc(vsa, sizeof(test8_allign));
    test_8->x = 5;
    test_8->c = 'l';
    TEST(test_8->x,5);
    TEST(MaxFreeBlockSize(vsa),40);

    test_16_another = (test16_allign *)VSAAlloc(vsa, sizeof(test16_allign));
    test_16_another->x = 9;
    test_16_another->z = "abc";
    TEST(test_16_another->x,9);
    TEST(MaxFreeBlockSize(vsa),16);

    test_8_another = (test8_allign *)VSAAlloc(vsa, sizeof(test8_allign));
    test_8_another->x = 50;
    test_8_another->c = 'l';
    TEST(test_8_another->x,50);
    TEST(MaxFreeBlockSize(vsa),0);

    VSAFree(test_8);
    TEST(MaxFreeBlockSize(vsa),8);
    VSAFree(test_16);
    TEST(MaxFreeBlockSize(vsa),16);

    test_24 = (test24_allign *)VSAAlloc(vsa, sizeof(test24_allign));
    test_24->x = 3;
    test_24->y = 9;
    test_24->z = &check;
    test_24->c = 'L';
    TEST(test_24->x,3);
    TEST(test_24->y,9);
    TEST(*test_24->z,8);

    VSAFree(test_24);
    TEST(MaxFreeBlockSize(vsa),32);

    free(pool);
    pool = NULL;

    return 0;
}
