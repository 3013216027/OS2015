#Process

- 内容: 甚么是进程; 程序和进程的区别; 进程的生命周期
 - 多道: 轮流使用CPU

- 进程模型
 - 某一时刻只有一个程序处于活动状态(单核)
 - 进程就是正运行着的程序的一个实例，包含了程序代码和当前的活动
 - 进程和程序的区别: 后者只是静态文本，前者是一个动态实体，且有自己的声明周期
 - 一个进程包含*3*部分: 进程控制块(*PCB*)，文本(*Text*)，数据块(*Data*)

- 进程的创建
 - 进程创建的时机: 系统初始化、程序发起系统调用、用户创建、批量任务发起
 - *fork*

- 进程的生命周期
 - 周期: *SNONE*&*SIDL*(*I*) => *R*/*T*/*S*/*RUNNING*(*A*) => *Zombie*(*Z*)
 - *Running*+*Ready*=*Runnable*, *Blocked*.
 	- 1. 进程等待输入(缺乏其它资源，进程**主动**放弃*CPU*资源，进入睡眠): *Running* => *Blocked*
	- 2. *Scheduler*(调度器) 从其它进程抢占*CPU*: *Running* => *Ready*
	- 3. *Scheduler*选择某进程(事实上，*Scheduler*有一个指针，指向谁就可让其运行): *Ready* => *Running*
	- 4. 输入准备好(资源准备完毕，由资源准备方(如其它进程)唤醒睡眠中的进程，进入队列等待*CPU*资源): *Blocked* => *Ready*
 - 进程终结(*Termination*)
	- 正常退出(自愿, *voluntary*)
	- 引发错误退出(自愿)
	- 致命错误退出(非自愿, *involuntary*)
	- 被杀死(非自愿, 注意，信号*9*,*SIGTERM*不可被屏蔽)

- 常用的相关系统调用
 - `fork()`, `vfork()`
 - `exec`: `execl()`, `execv()`, `execle()`, `execve()`, `execlp()`, `execvp()`
 - `wait()`, `waitpid()`
 - `exit()`
 - `system()`
