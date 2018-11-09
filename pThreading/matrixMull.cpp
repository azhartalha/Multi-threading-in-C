#define _CRT_SECURE_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <time.h>
#define n 1000

int x = 0;
int **res, **A, **B;

void * multiProcessing(void * args)
{
	int c = ((int *)args)[0];
	for (int i = 0; i < n; i++)
	{
		for (int j = c; j < n; j += 4)
		{
			res[i][j] = 0;
			for (int k = 0; k < n; k++)
				res[i][j] += A[i][j] * B[i][j];
		}
	}
	return NULL;
}

int ** MUL(int **A, int **B)
{
	clock_t start, end;
	start = clock();
	pthread_t id[4];
	res = (int **)malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++)
		res[i] = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < 4; i++)
	{
		int * j = (int *)malloc(sizeof(int));
		j[0] = i;
		pthread_create(&id[i], NULL, multiProcessing, (void *)j);
	}
	for (int i = 0; i < 4; i++)
		pthread_join(id[i], NULL);
	end = clock();
	printf("%f", ((double)(end - start)) / 1000);
	return res;
}

int matrixMUl()
{
	A = (int**)malloc(sizeof(int*)*n);
	for (int i = 0; i < n; i++)
		A[i] = (int *)malloc(sizeof(int)*n);
	B = (int**)malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++)
		B[i] = (int *)malloc(sizeof(int) * n);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			A[i][j] = 1;
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			B[i][j] = 1;
		}
	}
	res = MUL(A, B);
	_getch();
	return 0;
}