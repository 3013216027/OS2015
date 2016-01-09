# 06 Inter-Process Communications, IPC, 进程间通信

- 竞争条件: 两个或多个进程读写某些共享数据，而最后的结果取决于子进程运行的精确时序，称为竞争条件
- 临界区: 对共享内积存进行访问的程序片段称作临界区域或临界区

### Peterson解法

```c
#define FALSE 0
#define TRUE 1
#define N 2	/* 进程数量 */

int turn;	/* 轮到谁? */
int interested[N] = {0};	/* 初始化0 */

void enter_region(int pid); /* 进程pid为0或1 */
void leave_region(int pid);

void enter_region(int pid)
{
	int other = 1 - pid; /* 另一方 */
	interested[pid] = TRUE;
	turn = pid;
	while (turn == pid && interested[other] == TRUE);
}

void leave_region(int pid)
{
	interested[pid] = FALSE;
}

/*
   注意，上述while (turn == pid)的判断是有意义的
   当两个进程几乎同时enter_region表明他们想进入临界区时，
   turn会被设置成后来的那个进程的编号，
   此时先执行turn = pid的进程会因为turn != pid而调出循环，于是可以获得资源，
   而另一方则进入等待
 */
```

### Dining Philosophers Problem

- 哲学家问题
 - 有两种状态, 吃饭和思考
 - 五碗面, 管够. 五只筷子. 任一人要一双筷子才能吃面.
 - 临界资源是筷子(因为面是富余的)
 
 ```c
 #define N 5
 #define LEFT ((i + N - 1) % N)
 #define RIGHT ((i + 1) % N)
 #define THINKING 0
 #define HUNGRY 1
 #define EATING 2
 typedef int semaphore

 int state[N];
 semaphore mutex = 1;
 semaphore s[N];

 void test(int i)
 {
	 if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
	 {
		 state[i] = EATING;
		 up(&s[i]);
	 }
 }

 void take_forks(int i)
 {
	down(&mutex);
	state[i] = HUNGRY;
	test(i);
	up(&mutex);
	down(&s[i]);
 }

 void put_forks(int i)
 {
	down(&mutex);
	state[i] = THINKING;
	test(LEFT);
	test(RIGHT);
	up(&mutex);
 }

 void philosopher(int i)
 {
	while (TRUE)
	{
		think();
		take_forks(i); /* acquire two forks or block */
		eat();
		put_forks(i); /* put both forks back on table */
	}
 }
 /* .... */
 ```

### The Readers and Writers Problem

- 读者/写者问题
 - 有读者和写者，分别读取数据和写入数据
 - 多个读者可以同时读取数据，因为不会影响数据内容
 - 多个写者不允许同时写
 - 读者和写者不能混合操作，有读者在读则不允许写数据，反之亦然

 ```c
 typedef int semaphore;
 semaphore mutex = 1;
 semaphore db = 1;
 int rc = 0;

 void read()
 {
	while (TRUE)
	{
		down(&mutex);
		rc = rc + 1;
		if (rc == 1)
		{
			down(&db);
		}
		up(&mutex);

		read_data_base();
		
		down(&mutex);
		rc = rc - 1;
		if (rc == 0)
		{
			up(&db);
		}
		up(&mutex);

		use_data_read();
	}
 }

 void writer()
 {
	 while (TRUE)
	 {
		 think_up_data();
		 down(&db);
		 write_data_base();
		 up(&db);
	 }
 }
 ```

- **信号量不是整数，而是包括整数、P/V操作等等的一套编程方法!**

### Mutexes in pthread

| Thread call | Description |
|:-----------:|:-----------:|
|pthread_mutex_init|Create a mutex|
|pthread_mutex_destroy|Destroy an existing mutex|
|pthread_mutex_lock|Acquire a lock or block|
|pthread_mutex_trylock|Acquire a lock or fail|
|pthread_mutex_unlock|Release a lock|
|pthread_cond_init|Create a condition variable|
|pthread_cond_destroy|Destroy a condition variable|
|pthread_cond_wait|Block waiting for a signal|
|pthread_cond_signal|Signal another thread and wake it up|
|pthread_cond_broadcast|Signal multiple threads and wake all of them|

### Others

- Monitors 管程
- Message Passing 消息传递 => (Java synchronized)
- Barriers 障碍
