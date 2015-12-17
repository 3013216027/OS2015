- Author: zhengdongjian@tju.edu.cn
- Created Time: Sun 13 Dec 2015 10:27:15 AM CST

# 使用说明

## 文件描述
- `ipc.h`和`ipc.c`包含对`struct semun`的定义和`P``V`函数的定义
- `init.c`是初始化程序
- `putA.c`和`putB.c`分别为A产品和B产品的入库程序描述

## 运行说明
- 需要: `gcc`和`make`
- 运行`make`编译
- 运行`make clean`清除中间产生的文件
- 运行`./init`初始化
- 运行`./putA`启动A产品的入库操作
- 运行`./putB`启动B产品的入库操作

# 信号量部分的3个POSIX函数

## semget
- 创建一个新信号量或取得一个已有信号量的键
- `int semget(key_t key, int num_sems, int sem_flags);`
 - `key`: 键，整数值，不相关的进程可以通过它访问同一信号量
 - `num_sems`: 需要的信号量数目，大多数情况都为$1$
 - `sem_flags`: 类似`fopen`的标志，低$9$位是信号量权限。可以`|``IPC_CREAT`创建，即使得到一个已有信号量的键，也不会产生错误；继续`|``IPC_EXCL`则得到一个新的唯一信号量，如果已存在会报错
 - 返回值: 成功返回一个正整数值，即为信号量标识符；失败返回-1

## semop
- 用于改变信号量的值，即`semaphore operate`
- `int semop(int sem_id, struct sembuf* sem_ops, size_t num_sem_ops);`
 - `sem_id`是`semget`得到的返回的信号量标识符
 - `sem_ops`是指向一个结构数组的指针，根据标准，每个数组元素至少包含以下成员:
 ```c
 struct sembuf {
	 short sem_num; /* 信号量编号，单个信号量时为0 */
	 short sem_op; /* 信号量在一次操作中需要改变的值，一般为-1(P)或者+1(V) */
	 short sem_flg; /* 通常设置为SEM_UNDO, 操作系统会自动跟踪该信号量，在进程终止时，如果信号量没有释放，则操作系统自动释放 */
 }
 ```
## semctl
- 用于直接控制信号量
- `int semctl(int sem_id, int sem_num, int command, ...);`
 - `sem_id`: 也是`semget`得到的返回的信号量标识符
 - `sem_num`: 信号量编号，使用单个信号量时为0,
 - `command`: 要采取的动作, 常用两个，第一个是`SETVAL`，用来信号量初始赋值，第二个是`IPC_RMID`用于删除一个无用的信号量标识符
 - 如果有第四个参数，根据标准，应为一个`union semun`结构，由`X/OPEN`规范:
 ```c
 union semun {
	 int val;
	 struct semid_ds* buf;
	 unsigned short* array;
 }
 ```

# 共享内存部分的4个函数(shm, shared memory)

## shmget
- 创建共享内存
- `int shmget(key_t key, size_t size, int shmflg);`
 - `key`: 为共享内存段命名
 - `size`: 指定共享内存的大小
 - `shmflg`: 包含$9$位的权限标志，与`IPC_CREAT`或运算后才能创建一个新的共享内存段
 - 返回值: 一个共享内存标识符，该标识符将用于后续的共享内存函数

## shmat
- 第一次创建共享内存段(shmget)时，它不能被任何进程访问。`shmat`可以将其连接到一个进程的地址空间中，以此启用访问
- `void* shmat(int shm_id, const void* shm_addr, int shmflg);`
 - `shm_id`: 由`shmget`返回的共享内存标识符
 - `shm_addr`: 共享内存连接到当前进程中的地址位置，通常为一个空指针，表示让系统来选择。
 - `shmflg`: 一组位标志，`SHM_RND`和`shm_addr`联合使用，用来控制共享内存连接的地址，`SHM_RDONLY`使得连接的内存只读。多数情况下不需要控制共享内存连接的地址。
 - 返回值: 成功返回指向共享内存第一个字节的指针，失败返回-1

## shmdt
- 将共享内存从当前进程中分离

## shmctl
- 控制共享内存
- `int shmctl(int shm_id, int command, struct shmid_ds* buf);`
 - `shmid_ds`结构:
 ```c
 struct shmid_ds {
	 uid_t shm_perm.uid;
	 uid_t shm_perm.gid;
	 node_t shm_perm.mode;
 }
 ```
 - `shm_id`: `shmget`返回的共享内存标识符
 - `command`: 要采取的动作，`IPC_STAT`把`shmid_ds`结构中的数据设置为共享内存的当前关联值；`IPC_SET`如果进程有足够权限，把共享内存的当前关联值设置为`shmid_ds`结构中给定的值；`IPC_RMID`删除共享内存段
 - `buf`指向包含共享内存模式和访问权限的结构
 - 返回值: 成功返回0，失败返回-1
