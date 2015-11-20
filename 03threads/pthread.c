/* **********************************************

  File Name: pthread.c

  Author: zhengdongjian@tju.edu.cn

  Created Time: Fri 20 Nov 2015 02:07:02 PM CST

*********************************************** */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

#define NUMBER_OF_THREADS 10

void* print_hello_world(void* tid) {
	//print the thread id (tid) and exit
	printf("Hello world, greetings from thread %d, of tid %lld\n", *((int*)tid), (long long)pthread_self());
	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char* argv[]) {
	/* 主线程创建10个线程，然后退出 */
	pthread_t threads[NUMBER_OF_THREADS];
	int status, i;
	printf("main: %lld\n", (long long)pthread_self());
	for (i = 0; i < NUMBER_OF_THREADS; ++i) {
		printf("Main here, creating thread %d\n", i);
		status = pthread_create(threads + i, NULL, &print_hello_world, (void*)(&i));

		if (status != 0) {
			printf("Oops, pthread_create returned error code %d\n", status);
			break;
		}
		sched_yield();
		//pthread_yield();
		//pthread_join(threads[i], NULL);
	}
	return 0;
}
