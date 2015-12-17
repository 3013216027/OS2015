#05 Race-Semaphore

- 重点: *TSL* 算法，代码背下来
- 重点2: 信号量定义，忙等待里的定义，背

### Peterson解法
```c
#define FALSE 0
#define TRUE 1
#define N	/* 进程数量 */

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

### TSL(Test and Set Lock)

- `TSL RX, LOCK`: 测试并加锁, 将内存字`lock`读入寄存器`RX`，并在内存中存入一个非零值
 - 保证读和写该内存之间其它处理器不允许访问该内存, 执行此指令的`CPU`将锁住内存总线

```asm
enter_region:
	TSL REGISTER, LOCK
	CMP REGISTER, #0
	JNE enter_region
	RET

leave_region:
	MOVE REGISTER, #0
	RET
```

```asm
enter_region:
	MOVE REGISTER, #1
	XCHG REGISTER, LOCK
	CMP REGISTER, #0
	JNE enter_region
	RET

leave_region:
	MOVE REGISTER, #0
	RET
```

### 生产者-消费者问题

```c
#define TRUE 1
#define FALSE 0

#define N 100	/* 槽容量 */
typedef int semaphore	/* 信号量是一种特殊的整型数据，但绝非整数.. */
semaphore mutex = 1;	/* 控制对临界区的访问 */
semaphore empty = N;
semaphore full = 0;

void producer();
void consumer();

void producer()
{
	int item;
	while (TRUE)
	{
		item = produce_item();
		down(&empty);
		down(&mutex);
		insert_item(item);
		up(&mutex);
		up(&full);
	}
}

void consumer()
{
	int item;
	while (TRUE)
	{
		down(&full);
		down(&mutex);
		item = remove_item();
		up(&mutex);
		up(&empty);
		consume_item(item);
	}
}
```
