# 页面置换算法

- 最优页面置换算法
 - 优先置换那些需要在更多指令后访问的页面。如页面1在600条指令后会再次访问，页面2在800条指令后会再次访问，则置换页面2
 - 不可能实现

- 最近未使用页面置换算法 NRU, Not Recently Used
 - 使用两个标志位，`R`表示被访问，`M`表示被修改。时钟中断定期清除`R`位(如果之后该页面被访问，则`R`位又会被置为1，以此达到刷新的目的。而`M`位则是在需要淘汰该页面时，如果`M`位是1，表示从硬盘读出后已经被修改过了，那么淘汰时就要先写会硬盘，否则就只需要直接丢弃该页面即可)。则页面可分为4类：

 | 类编号 | 状态 | RM标志位 |
 |:----:|:-----:|:-----:|
 | 第0类 | 没有被访问，没有被修改 | 00 |
 | 第1类 | 没有被访问，已被修改 | 01 |
 | 第2类 | 已被访问，没有被修改 | 10 |
 | 第3类 | 已被访问，已被修改 | 11 |
 - 从类编号最小的非空类中挑选一个页面淘汰之

- 先进先出页面置换算法 FIFO, First In First Out
 - 由操作系统维护一个所有当前在内存中的页面的链表，最新进入的页面放在表尾，最久进入的页面放在表头。当发生缺页中断时，就淘汰表头的页面，并把心调入的页面加到表尾
 - 很少采用纯粹的`FIFO`算法

- 第二次机会页面置换算法 second chance
 - 改进`FIFO`算法，检查最老页面的`R`位，若为0则直接置换掉，否则清除其`R`位并放入表尾，继续搜索直到找到满足条件的页面可供置换
 - 注意，如果所有链表中的页面最近都被访问过(`R`位都为1)，会发生一次彻底的对`R`位的“刷新”，此时退化为`FIFO`算法

- 时钟页面置换算法
 - 把第二次机会页面置换算法的链表摆成一个环形(循环队列的样子)，维护一个指针指向最老页面，如果`R`为0，就用新页面直接替换它的位置，并且指针前移，否则`R`置为0，指针前移，然后继续查找

- 最近最少使用页面置换算法 LRU, Least Recently Used
 - 选取最近最少使用的页面，替换之
 - 直接维护和其它方法使用的一样的链表，代价高昂。
 - 硬件实现之一：设置一个计数器C，页面被访问时，将C一并计入页面内，发生缺页中断时就遍历查找页面中保存的数值最小的那一个。 ==> 相当于一个时间戳
 - 硬件实现之二：设置一个`n x n`的矩阵，初始化全0，访问页框(Page Frame)k时，置第k行全为1，再置第k列全为0，发生缺页中断时，检查代表的二进制数值最小的行，便对应于最近最少使用的页面

- 用软件模拟LRU
 - 其中一种实现方案称为NFU, Not Frequently Used，最不常用算法
 - 为每个页面设置一个软件计数器，初始化为0，每次时钟中断时，遍历每个页面，如果标志位`R`为1，则计数器加1。发生缺页中断时，置换计数器值最小的页面
 - bug：不能*忘记*之前的任何访问信息。如在一个多遍的编译器中，第一遍扫描时间最长，则其中被频繁使用的页面尽管在后面若干遍中可能再也不使用，却因为计数器的数值从不减小而一直无法被淘汰
 - **改进**：在`R`位加入之前先右移一位，然后如果`R`位为0则置最高位(而不是从低位加1)为1。=> 此时的比较大小有了一个变化，那就是越近被访问的页面的计数器数值，被增加的数值1的“位权”越大，比如两个页面，计数器分别为1111和0000，最近一个时钟周期的检查发现后者的`R`位为1而前者为0，那么这个时钟周期后，计数器数值分别变为0111和1000，第二个页面仅仅由于最后一次的访问，数值就超过了第一个页面。 => 这样的修改是有意义的~

- 工作集页面置换算法
 - 一个进程当前正在使用的页面的集合称为它的**工作集**
 - 请求调页：需要时才调入；预先调页：预先装入工作集页面
 - 颠簸：每执行几条指令就发生一次缺页中断
 - 发生缺页中断时，检查每个表项的`R`位，为1则更新其“上次使用时间”域，其应该属于该进程的工作集之中；否则根据其“上次使用时间”来计算生存周期，然后和`t`比较(`t`是生成周期，横跨多个时钟滴答)，若生存周期大于`t`就置换。继续扫描整个表已完成最后的更新。

- 工作集时钟页面置换算法 WSClock, Working Set Clock
 - 和时钟页面置换算法所做的改进类似，加载的页面组件排列成一个环状，缺页中断发生时，指针一次检查页面，如果`R`标志位为1则继续，否则如果其生存周期大于`t`，则考虑其`W`位，为1则说明其不干净(dirty)，指针继续前进，否则可以申请该页面对应的页框。考虑更新完整个环后，为了降低磁盘阻塞，需要设置一个限制，即最大只允许写回n个页面。最后指针回到起点，此时有两种情况：
 	- 1、至少调度了一次写操作，指针可以继续移动寻找一个干净页面，因为之前的一遍扫描已经有至少一个或者多个超过生存周期的页面变成干净的了
	- 2、没有调度过写操作，则当前所有页面都在工作集中，于是可以随便置换一个干净页面来使用，如果不存在干净页面，则置换当前页面