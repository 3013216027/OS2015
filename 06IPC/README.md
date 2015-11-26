# 06 Inter-Process Communications

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
