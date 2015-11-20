/* **********************************************

  File Name: main.c

  Author: zhengdongjian@tju.edu.cn

  Created Time: Fri 20 Nov 2015 04:28:57 PM CST

*********************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#define NUMBER_OF_THREADS 100
#define MAX_N 100000000 /* ensure that MAX_N % NUMBER_OF_THREADS = 0 */
#define MAX_N_SQR 1e16

/* struct to define how each thread works */
typedef struct
{
	int start;
	double* sum;
} Node;

void*
gao(void* nod)
{
	Node* yy = (Node*)nod;
	int i = yy->start;
	double sm = 0.0;
	for (; i <= MAX_N; i += NUMBER_OF_THREADS)
	{
		sm += 1.0 / (MAX_N_SQR + (i + 0.5) * (i + 0.5));
	}
	*(yy->sum) += sm;
	pthread_exit(NULL);
	return NULL;
}

int
main(int argc, char* argv[])
{
	double sum = 0;
	Node my[NUMBER_OF_THREADS];
	pthread_t tid[NUMBER_OF_THREADS];
	int i;

	for (i = 0; i < NUMBER_OF_THREADS; ++i)
	{
		my[i].start = i;
		my[i].sum = &sum;
		int status = pthread_create(tid + i, NULL, &gao, (void*)(my + i));

		if (status != 0)
		{
			printf("Oops! Error occured when creating thread %d.\n", i);
			exit(1);
		}
	}
	
	/* wating for all threads exit */
	for (i = 0; i < NUMBER_OF_THREADS; ++i)
	{
		pthread_join(tid[i], NULL);
	}
	
	sum *= 4.0 * MAX_N;
	printf("PI approximately equals to %.8f\n", sum);

	return 0;
}
/* 0.056s */
