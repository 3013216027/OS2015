# Threads 线程

- 几个实例
 - 例1: 使用*Word*编辑一个超大文件 => 键盘，硬盘读写，排版轮替
 - 例2: 网页服务器 => 高吞吐量的资源访问, 线程之间可以共享资源，而进程之前占用的资源则不能共享，需要相同资源时必有一方需要等待.
 - 总结: 进程太**笨**, 需要完整的声明周期，尤其在需要大量的任务处理时(如*Alipay*每秒$60k$的交易量)，非常慢。而线程的启动则快得多，一般而言，是进程启动时间的$1\over{10}$，相差$1$个数量级

- 线程模型
 - 1. 单线程模型
 - 2. 多线程模型，轮流执行不同线程
 - 和进程的对比

 | 进程 | 线程 |
 |:----------------:|:----------------:|
 |地址空间, Address space|程序计数器,Program counter|
 |全局变量, Global variables|寄存器, Registers|
 |打开的文件, Open files|栈, Stack|
 |子进程, Child processes|状态, State(主要为寄存器现场)|
 |Pending alarms||
 |Signals and signal handlers||
 |Accounting information||

 - *POSIX Threads*

 | 线程调用, Thread call | 功能 |
 |:---------------------:|:--------------------:|
 | pthread_create | 创建新线程 |
 | pthread_exit | 结束线程 |
 | pthread_join | 等待线程结束 |
 | (!)pthread_yield | 释放CPU资源 |
 | pthread_attr_init | 创建并初始化一个线程(属性)结构, attribute structure |
 | pthread_attr_destroy | 移除线程(属性)结构 |

- 线程的实现
 - 方案*1*: 在进程中实现，但是，当某个线程缺乏资源时，线程进入`Blocked`，此时会导致整个进程阻塞; 优点是不同的进程的线程可以个性化，采用不同的调度策略.
 - 方案*2*: 在系统内核中实现，优缺点与上述相反。
 - 混合方案*3*: 分割用户线程(*User threads*)和内核线程(*Kernel thread*)，若干个用户线程可被分入一个组，由一个内核线程控制.

- *Thread Pop-up* & others~
