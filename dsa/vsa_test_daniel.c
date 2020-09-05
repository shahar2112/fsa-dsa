#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */

#include "vsa.h"
#define TEST(real,expected) (real == expected ? printf("%d, Pass\n", expected) : printf("%d, Fail\n",expected))
#pragma pack (1)

typedef struct vsa_test10
{
	int x;
	int y;
	short z;
}test10_t;

typedef struct vsa_test23
{
	size_t a;
	size_t b;
	int c;
	short d;
	char e;
}test23_t;

typedef struct vsa_test28
{
	size_t f;
	size_t g;
	size_t h;
	int i;
}test28_t;

int main()
{
	void *data = NULL;
	vsa_t* Pool = NULL;
	test10_t *TestVar10 = NULL;
	test23_t *TestVar23 = NULL;
	test28_t *TestVar28 = NULL;
	
	int *num1 = 0;
	int *num2 = 0;
	int *num3 = 0;
	int *num4 = 0;

	data = malloc(100);
	if(NULL == data)
	{
		fprintf(stderr, "Allocation failed");
		exit(1);
	}

	Pool = VSAInit(data, 100);

	TEST(MaxFreeBlockSize(Pool),80);

	TestVar10 = (test10_t *)VSAAlloc(Pool, 10);
	TestVar10->x = 1;
	TestVar10->y = 2;
	TestVar10->z = 3;
	
	TEST(TestVar10->x,1);
	TEST(TestVar10->y,2);
	TEST(TestVar10->z,3);

	TEST(MaxFreeBlockSize(Pool),56);
	TestVar23 = (test23_t *)VSAAlloc(Pool, 23);
	TestVar23->a = 4;
	TestVar23->b = 5;
	TestVar23->c = 6;
	TestVar23->d = 7;
	TestVar23->e = 'A';

	TEST(TestVar23->a,4);
	TEST(TestVar23->b,5);
	TEST(TestVar23->c,6);
	TEST(TestVar23->d,7);
	TEST(TestVar23->e,'A');

	TEST(MaxFreeBlockSize(Pool),24);

	num1 = (int *)VSAAlloc(Pool,4);
	*num1 = 8;
	TEST(*num1,8);

	TEST(MaxFreeBlockSize(Pool),8);

	num2 = (int *)VSAAlloc(Pool,4);
	*num2 = 9;
	TEST(*num2,9);

	VSAFree(TestVar23);

	TEST(MaxFreeBlockSize(Pool),24);

	VSAFree(num1);

	TEST(MaxFreeBlockSize(Pool),24);

	TestVar28 = (test28_t *)VSAAlloc(Pool, 28);
	TestVar28->f = 11;
	TestVar28->g = 12;

	TEST(TestVar28->f,11);
	TEST(TestVar28->g,12);

	TEST(MaxFreeBlockSize(Pool),0);

	free(data);
	data = NULL;
	return 0;
	
}

