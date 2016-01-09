/* **********************************************

  File Name: init.c

  Author: zhengdongjian@tju.edu.cn

  Created Time: Sun 13 Dec 2015 01:14:46 AM CST

*********************************************** */

#include "ipc.h"

#define MAXN 2
#define MAXM 3

int
main() {
	int n = MAXN;
	int m = MAXM;
	int i, item, shmid;
	semaphore mutex;
	semaphore dif1;
	semaphore dif2;
	void* shared_memory = NULL;
	struct shared_use_st* shared_stuff;

	if ((mutex = semget((key_t)KEY_MUTEX, 1, 0666|IPC_CREAT)) == -1) {
		fprintf(stderr, "1Failed to create semaphore!\n");
		exit(EXIT_FAILURE);
	}
	if ((dif1 = semget((key_t)KEY_EMPTY, 1, 0666|IPC_CREAT)) == -1) {
		fprintf(stderr, "2Failed to create semaphore!\n");
		exit(EXIT_FAILURE);
	}
	if ((dif2 = semget((key_t)KEY_FULL, 1, 0666|IPC_CREAT)) == -1) {
		fprintf(stderr, "3Failed to create semaphore!\n");
		exit(EXIT_FAILURE);
	}
	if ((shmid = shmget((key_t)KEY_SHM, sizeof(struct shared_use_st), 0666|IPC_CREAT)) == -1) {
		fprintf(stderr, "4Failed to create semaphore!\n");
		exit(EXIT_FAILURE);
	}
	if ((shared_memory = shmat(shmid, (void*)0, 0)) == (void*)(-1)) {
		fprintf(stderr, "shmat failed!\n");
		exit(EXIT_FAILURE);
	}
	shared_stuff = (struct shared_use_st*)shared_memory;

	union semun sem_union;
	sem_union.val = 1;
	if (semctl(mutex, 0, SETVAL, sem_union) == -1) {
		fprintf(stderr, "Failed to set semaphore!\n");
		exit(EXIT_FAILURE);
	}
	sem_union.val = n - 1;
	if (semctl(dif1, 0, SETVAL, sem_union) == -1) {
		fprintf(stderr, "Failed to set semaphore!\n");
		exit(EXIT_FAILURE);
	}
	sem_union.val = m - 1;
	if (semctl(dif2, 0, SETVAL, sem_union) == -1) {
		fprintf(stderr, "Failed to set semaphore!\n");
		exit(EXIT_FAILURE);
	}
	shared_stuff->low = 0;
	shared_stuff->high = 0;
	exit(EXIT_SUCCESS);
}
