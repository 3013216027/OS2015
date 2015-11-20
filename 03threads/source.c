/* **********************************************

  File Name: source.c

  Author: zhengdongjian@tju.edu.cn

  Created Time: Fri 20 Nov 2015 07:07:12 PM CST

*********************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 10

void *print_hello_world(void *tid)
{
	printf("Hello world, greetings from thread %d\n", *(int*)tid);
	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t threads[NUMBER_OF_THREADS];
	int status, i;

	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		printf("Main here, creating thread %d\n", i);
		status = pthread_create(&threads[i], NULL, print_hello_world, (void*)(&i));

		if (status != 0) {
			printf("Oops, pthread_create return error %d\n", status);
			exit(1);
		}
	}
	exit(0);
}
