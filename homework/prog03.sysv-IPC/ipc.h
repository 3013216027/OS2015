/* **********************************************

  File Name: ipc.h

  Author: zhengdongjian@tju.edu.cn

  Created Time: Sun 13 Dec 2015 12:53:51 AM CST

*********************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>

#define KEY_MUTEX 100
#define KEY_FULL 102
#define KEY_EMPTY 101
#define KEY_SHM 200
#define BUFFER_SIZE 10

typedef int semaphore;

union semun {
	int val;
	struct semid_ds* buf;
	unsigned short int* array;
};

/* 共享内存 */
struct shared_use_st {
	int buffer[BUFFER_SIZE]; /* 循环队列表示仓库 */
	int low; /* A产品可入库位置 */
	int high; /* B产品可入库位置 */
	int cur; /* 当前位置 */
};

extern int
sem_p(semaphore sem_id); /* operate p */

extern int
sem_v(semaphore sem_id); /* operate v */
