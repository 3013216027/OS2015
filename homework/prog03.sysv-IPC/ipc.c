/* **********************************************

  File Name: ipc.c

  Author: zhengdongjian@tju.edu.cn

  Created Time: Sun 13 Dec 2015 01:07:23 AM CST

*********************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include "ipc.h"

static int sem_set(semaphore sem_id, int val) {
	union semun sem_union;
	sem_union.val = val;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
		return 0;
	}
	return 1;
}

static void sem_del(semaphore sem_id) {
	union semun sem_union;
	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
		fprintf(stderr, "Failed to delete semaphore!\n");
	}
}

/* operate p */
int
sem_p(semaphore sem_id) {
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1; /* p */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "Semaphore Operate failed!\n");
		return 0;
	}
	return 1;
}

int
sem_v(semaphore sem_id) {
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1; /* v */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "Semaphore_V failed!\n");
		return 0;
	}
	return 1;
}

static int
shm_create(int key) {
	return shmget((key_t)key, sizeof(struct shared_use_st), 0666|IPC_CREAT);
}

