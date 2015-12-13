/* **********************************************

  File Name: putA.c

  Author: zhengdongjian@tju.edu.cn

  Created Time: Sun 13 Dec 2015 01:23:30 AM CST

*********************************************** */

#include "ipc.h"

void
print(struct shared_use_st* shared_stuff) {
	printf("put 1 product A.\n");
	printf("Current state: %d product A(s)\t%d product B(s).\n", shared_stuff->high, shared_stuff->low);
}

int
main() {
	int i, item, shmid;
	semaphore mutex;
	semaphore dif1;
	semaphore dif2;
	void* shared_memory = NULL;
	struct shared_use_st* shared_stuff = NULL;

	if ((mutex = semget((key_t)KEY_MUTEX, 1, 0666|IPC_CREAT)) == -1) {
		fprintf(stderr, "Failed to create semaphore!\n");
		exit(EXIT_FAILURE);
	}
	if ((dif1 = semget((key_t)KEY_EMPTY, 1, 0666|IPC_CREAT)) == -1) {
		fprintf(stderr, "Failed to create semaphore!\n");
		exit(EXIT_FAILURE);
	}
	if ((dif2 = semget((key_t)KEY_FULL, 1, 0666|IPC_CREAT)) == -1) {
		fprintf(stderr, "Failed to create semaphore!\n");
		exit(EXIT_FAILURE);
	}
	if ((shmid = shmget((key_t)KEY_SHM, 1, 0666|IPC_CREAT)) == -1) {
		fprintf(stderr, "Failed to create semaphore!\n");
		exit(EXIT_FAILURE);
	}
	if ((shared_memory = shmat(shmid, NULL, 0)) == (void*)(-1)) {
		fprintf(stderr, "shmat failed!\n");
		exit(EXIT_FAILURE);
	}
	shared_stuff = (struct shared_use_st*)shared_memory;

	for (i = 0; i < 30; ++i) {
		sleep(1);
		sem_p(dif2);
		sem_p(mutex);
		shared_stuff->high++;
		print(shared_stuff);
		sem_v(dif1);
		sem_v(mutex);
	}
	
	exit(EXIT_SUCCESS);
}
