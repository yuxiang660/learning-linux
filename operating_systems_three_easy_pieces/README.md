# 配置环境
## 安装“XV6”系统
* Ubuntu下安装qemu
   > sudo apt update && sudo apt-get install qemu-system
   * 安装完成后需要支持`qemu-system-i386`或者`qemu-system-x86_64`命令，否则会出现`Error: Couldn't find a working QEMU executable.`错误。可自行搜索相关依赖
* 安装xv6
   > git clone git://github.com/mit-pdos/xv6-public.git
   > make
* 启动xv6
   > make qemu
   * 在xv6目录下输入上面的命令，即可出现如下xv6窗口：
   ![xv6](./pictures/xv6.png)

# 第2章 操作系统介绍
## 关键问题
* 如何将资源虚拟化？
   * 操作系统通过哪些机制和策略来实现虚拟化？
   * 操作系统如何有效地实现虚拟化？
   * 需要哪些硬件支持？

操作系统将物理资源(如处理器、内存或磁盘)转换为更通用、更强大且更易于使用的虚拟形式。

## 虚拟化CPU
* 所谓的虚拟化CPU，即系统将单个CPU(或其中一小部分)转换为看似无限数量的CPU
* [例子"virtualizing_cpu"](./code/virtualizing_cpu)可以在一个CPU上同时运行多个程序

## 虚拟化内存
* 每个进程访问自己的私有虚拟地址空间，操作系统以某种方式映射到机器的物理内存上
* [例子"virtualizing_memory"](./code/virtualizing_memory)可以在同一个地址(虚拟地址)上读写数据
   ```bash
   > make run
   (480896) memory address of static_var: 0x555555558010
   (480896) memory address of p: 0x5555555596b0

   (480897) memory address of static_var: 0x555555558010
   (480897) memory address of p: 0x5555555596b0
   ```

## 并发
### 关键问题
* 如何构建正确的并发程序？
   * 如果同一个内存空间中有很多并发执行的线程，如何构建一个正确的工作程序？
   * 操作系统需要什么原语？
   * 硬件应该提供哪些机制？
   * 我们如何利用它们来解决并发问题？

并发程序有可能会导致一些深刻而有趣的问题，如：
* [例子"concurrency_thread"](./code/concurrency_thread)展示了并发运行时有可能出现的意外(计数器的值不是期望值)

## 持久性
操作系统通过管理磁盘的软件(文件系统file system)来持久地为用户保存数据。不像操作系统为CPU和内存提供的抽象，操作系统不会为每个应用程序创建专用的虚拟磁盘。相反，它假设用户经常需要共享文件中的信息。
### 关键问题
* 如何持久地存储数据？
   * 持久性需要哪些技术才能正确地实现？
   * 需要哪些机制和策略才能高性能地实现
   * 面对硬件和软件故障，可靠性如何实现？

* [例子"persistence_io"](./code/persistence_io)通过系统调用向磁盘中记录了`hello`信息。

## 设计目标
* 操作系统能做什么
   * 取得CPU、内存和磁盘等物理资源，并对它们进行虚拟化
   * 处理与并发有关的麻烦且棘手的问题
   * 持久地存储文件，从而使它们长期安全
* 设计操作系统的要求
   * 提供高性能
   * 在应用程序之间以及在OS和应用程序之间提供保护
   * 高度可靠，可不间断运行
   * 安全性、移动性

# 第4章 抽象：进程
## 关键问题
* 如何提供有许多CPU的假象？
   * 时分共享
      * CPU在时间上划分给多个任务
   * 空分共享
      * 磁盘在空间上划分给多个任务

* 操作系统以什么策略做出调度决定？
   * 历史信息
      * 哪个程序在最后一分钟要运行更多？
   * 工作负载
      * 运行什么类型的程序？
   * 性能指标
      * 系统是否针对交互式性能或吞吐量进行优化？

* 什么是操作系统的策略和机制分离？
   * 一个通用的设计范式是将高级策略与低级机制分开
   * 机制是解决how问题
      * 例如，操作系统如何执行上下文切换
   * 策略是解决which问题
      * 例如操作系统现在应该运行哪个进程
   * 将策略和机制分开可以轻松针对某个策略改变策略，而不必重新考虑机制

## 什么是进程？
操作系统为正在运行的程序提供的抽象，就是进程。进程的机器状态有下面几个部分组成：
* 内存
   * 指令
   * 数据
* 寄存器
   * PC寄存器
      * 告诉我们程序当前正在执行哪个指令
   * 栈指针stack pointer 和帧指针frame pointer
      * 用于管理函数参数栈、局部变量和返回地址
* 持久存储设备
   * 当前打开的文件列表

## 进程状态
* 运行running
   * 进程正在处理器上执行指令
* 就绪ready
   * 进程已准备好运行，但由于某种原因，操作系统选择不在此时运行
* 阻塞blocked
   * 进程执行了某种操作，直到发生其他事件时才会准备运行

![process_state_transitions](./pictures/process_state_transitions.png)
![process_status](./pictures/process_status.png)

上表中，Process0发起I/O并被阻塞，等待I/O完成。例如，当从磁盘读取数据或等待网络数据包时，进程会被阻塞。OS发现Process0不使用CPU，则开始运行Process1。当Process1运行时，I/O完成，将Process0移回就绪状态。最后，Process1结束，Process0运行，然后完成。OS在上面步骤中做了以下两个决策：
* 决定在Process0发出I/O时，运行Process1
* 当I/O完成时，系统决定不切回Process0

## 进程数据结构
操作系统有一些关键的数据结构来跟踪进程的状态，称为进程列表(process list)或者进程控制块(Process Control Block, PCB)。下面是xv6内核中每个进程的信息：
```c
// the registers xv6 will save and restore
// to stop and subsequently restart a process
struct context
{
   int eip;
   int esp;
   int ebx;
   int ecx;
   int edx;
   int esi;
   int edi;
   int ebp;
};
// the different states a process can be in
enum proc_state
{
   UNUSED,
   EMBRYO,
   SLEEPING,
   RUNNABLE,
   RUNNING,
   ZOMBIE
};
// the information xv6 tracks about each process
// including its register context and state
struct proc
{
   char *mem;    // Start of process memory
   uint sz;      // Size of process memory
   char *kstack; // Bottom of kernel stack for this process
   enum proc_state state;      // Process state
   int pid;                    // Process ID
   struct proc *parent;        // Parent process
   void *chan;                 // If non-zero, sleeping on chan
   int killed;                 // If non-zero, have been killed
   struct file *ofile[NOFILE]; // Open files
   struct inode *cwd;          // Current directory
   struct context context;     // Switch here to run process
   struct trapframe *tf;       // Trap frame for the current interrupt
};
```
