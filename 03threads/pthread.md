- 主函数中创建了$10$个线程，`pthread_create(pthread_t*, attr, void* (*)(void*), void*)`创建一个线程，成功返回$0$
 - 特别注意后面的`pthread_join()`，它会等待指定线程执行结束(`pthread_exit`)，如果不用`pthread_join`，实测中发现会出现以下问题:
 	- 无法人为保证各线程的执行顺序，于是主线程可能优先执行，主程序执行完毕，创建的所有$10$个线程都没有能够获得`CPU`资源，程序正常结束
	- 上述问题在使用循环创建多个线程时极容易出现，不知道为啥= =
 - `pthread_create`创建成功后，立即执行函数指针指定函数，返回值为`void*`，参数有且仅有一个`void*`，需要多参数可以打包结构体，参数由`pthread_create`的最后一个参数指定

- `pthread`相关常用命令和使用样例
 - `pthread_create`
  ```c
  void*
  gao(void* tid) {
	  printf("Hello, this is tid no.%d\n", *(int*)tid);
	  pthread_exit(tid);
	  return NULL;
  }
  int
  main(int argc, char* argv[]) {
	  pthread_t tid;
	  int idx = 1;
	  printf("main: %d\n", gettid());
	  pthread_create(&tid, NULL, &gao, (void*)(&idx)); //pthread_create(&tid, NULL, gao, (void*)(&idx));
	  return 0;
  }
  ```
 - `pthread_join`: 见例程`pthread.c`
 - `pthread_exit`: 见例程`pthread.c`
 - `pthread_yield`: 让调用该函数的线程释放`CPU`资源，**非标准**函数，应该使用`sched.h`中声明的`sched_yield`代替之。后者可以让`schelduer`将其放到队列的最后，详见`man sched_yield`
 - `pthread_attr_init`: 创建并初始化一个线程结构，`pthread_create`的第二个参数，给`pthread_create`第二个参数传递`NULL`时会自动使用一个默认属性结构
 - `pthread_attr_destroy(pthread_attr_t*) `与上述对应，移除参数的线程属性结构
