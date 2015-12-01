/* **********************************************

  File Name: pc.c

  Author: zhengdongjian@tju.edu.cn

  Created Time: Tue 01 Dec 2015 10:07:46 AM CST

*********************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10

pthread_mutex_t mutex;
pthread_cond_t condc, condp; //producer & cosumer
int buffer = 0;

void* produce(void* ptr);
void* consume(void* ptr);

void* produce(void* ptr)
{
	int i;
	for (i = 1; i <= MAX; i++)
	{
		pthread_mutex_lock(&mutex);
		while (buffer != 0)
		{
			pthread_cond_wait(&condp, &mutex);
		}
		fprintf(stdout, "produce product %d\n", i);
		buffer = i;
		pthread_cond_signal(&condc);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

void* consume(void* ptr)
{
	int i;
	for (i = 1; i <= MAX; i++)
	{
		pthread_mutex_lock(&mutex);
		while (buffer == 0)
		{
			pthread_cond_wait(&condc, &mutex);
		}
		fprintf(stdout, "consume product %d\n", buffer);
		buffer = 0;
		pthread_cond_signal(&condp);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t pro, con;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condc, NULL);
	pthread_cond_init(&condp, NULL);
	pthread_create(&con, NULL, consume, NULL);
	pthread_create(&pro, NULL, produce, NULL);
	pthread_join(pro, NULL);
	pthread_join(con, NULL);
	pthread_cond_destroy(&condc);
	pthread_cond_destroy(&condp);
	pthread_mutex_destroy(&mutex);

	return 0;
}
