#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

int lastAcessed = 0;

void* function1(void* arr)
{
	for (int i = 0; i < 100; i += 2)
	{
		while (lastAcessed == 1)
			sched_yield();
		printf("\nFunction 1: %d", ((int *)arr)[i]);
		lastAcessed = 1;
	}
	return NULL;
}

void* function2(void* arr)
{
	for (int i = 1; i < 100; i += 2)
	{
		while (lastAcessed == 0)
			sched_yield();
		printf("\nFunction 2: %d", ((int *)arr)[i]);
		lastAcessed = 0;
	}
	return NULL;
}

int sample()
{
	int i, arr[100];
	pthread_t id1, id2;
	for (int i = 0; i < 100; i++)
	{
		arr[i] = i + 1;
	}
	
	pthread_create(&id1, NULL, function2, (void *)arr);
	pthread_create(&id2, NULL, function1, (void *)arr);
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	_getch();
	return 0;
}