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


