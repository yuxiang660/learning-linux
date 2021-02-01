# 第1章 温故而知新
## 系统软件
* 平台性软件, 如：操作系统内核，驱动程序，运行库等
* 程序开发软件，如：编译器，汇编器，链接器等
![computer_arch](./pictures/computer_arch.png)

## 为什么要使用虚拟地址
使用物理地址的缺点有：
* 地址空间不隔离
* 内存使用效率低
* 程序运行的地址不确定

第一个和第三个问题可以用简单的分段(Segmentation，对每个进程内存映射)映射解决，但是第二个问题就必须用到分页(Paging)技术。分页技术将内存段进一步划分成页，针对每个页进行映射，就可以结合磁盘存储，达到内存的高效利用。
![virtual_memory](./pictures/virtual_memory.png)

## 线程基础
### 线程的访问权限
* 公共部分<br>
   * 可以访问进程内存里的所有数据
* 私有部分<br>
   * 栈(尽管并非完全无法被其他线程访问，但一般情况下仍然可以认为是私有的数据)
   * 线程局部存储(Thread Local Storage, TLS)
   * 寄存器(包括PC寄存器)

![thread](./pictures/thread.png)
![thread_privilege](./pictures/thread_privilege.png)

### 线程调度
线程的三个状态：
* 运行(Running)
* 就绪(Ready)
* 等待(Waiting)

调度方法：
* 优先级调度：低优先级的线程需要等待高优先级线程执行完后才执行
* 轮转法：各线程公平地执行一段时间

如何改变优先级，以防止低优先级线程无法执行：
* 用户指定优先级
* 根据进入等待状态的频繁程度提升或降低优先级
* 长时间得不到执行而被提升优先级

不可抢占(Preemption)线程中，线程主动放弃的两种情况：
* 当线程试图等待某事件时(I/O等)
* 线程主动放弃时间片

### Linux的线程实现
* `fork`
   * 复制当前进程，注意是写时复制
* `exec`
   * 使用新的可执行镜像覆盖当前可执行镜像
   * 因此，`fork`和`exec`配合使用，可以新建一个全新的进程
* `clone`
   * 创建子进程并从指定位置开始执行
   * 不同与`fork`，`clone`运行子进程和父进程共享资源，例如，内存，文件描述符等。因此`clone`更像新建了一个线程

### 同步与锁
我们有各种锁实现同步机制，各种机制都有各自的特点。
* 二元信号量(Binary Semaphore)
   * 二元信号量有两种状态，且状态变化时原子的。因此不同线程可以通过检查/修改此二元信号量，达到同步
   * 例子，[sem_process](./code/sem_process)
* 互斥量(Mutex)
   * 类似与二元信号量，互斥量的变化也是原子的，且也是二元的
   * 不同于二元信号量，互斥量的获取与释放必须是同一个线程
   * 例子：
      * [mutex_process](./code/mutex_process)。这个例子利用mutex和shared memory实现了进程间的同步
      * [mutex_process_fail](./code/mutex_process_fail)。这个例子只用了mutex，无法实现进程间的同步。单用linux的mutex只能实现线程同步，可参见例子：[mutex_thread](./code/mutex_thread)。
* 临界区(Critical Section)
   * 临界区和互斥量与信号量的区别在于，互斥量和信号量在系统的任何进程里都是可见的，另一个进程试图去获取该锁是合法的。然而，临界区的作用范围仅限于本进程。
* 读写锁(Read-Write Lock)
   * 读锁是共享的(shared)，而写锁是独占的(unique)
   * C++中的shared_lock和unique_lock，就是读写锁
   * 例子，[rwlock](./code/rwlock)
* 条件变量(Condition Variable)
   * 使用条件变量可以让许多线程一起等待某个事件的发生，当事件发生时，所有的线程可以一起恢复执行，而mutex却不行。
   * C++的`std::condition_variable`需要配合`unique_lock`使用，在`wait`的时候会释放`unique_lock`并阻塞，当条件满足后，阻塞解除，重新获取锁。例子，[cond_var](./code/cond_var)
