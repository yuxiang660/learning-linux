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

### 可重入(Reentrant)与线程安全
一个函数被重入，只有两种情况：
* 多个线程同时执行这个函数
* 函数自身(可能是经过多层调用之后)调用自身
一个函数可重入，必须有如下特点：
* 不使用任何局部静态或全局的非const变量
* 不返回任何局部静态或全局的非const变量
* 仅依赖于调用方提供的参数
* 不依赖任何单个资源的锁(mutex等)
* 不调用任何不可重入的函数
* 一个可重入的函数可以在多线程环境下放心使用
* 可重入函数一定是线程安全的，但是线程安全的函数(如加锁保护)不一定是可重入函数

### 过度优化
即使我们正确使用了锁，由于缓存(寄存器)和编译器优化的存在，仍然会出现不同步的事情发生。
* 每个线程都有自己的寄存器，对寄存器的读写无法不能用锁保护，如果没有及时对内存进行写入操作，就有可能发生不同步
* 编译器为了优化代码，有可能会交换代码顺序，因此也会造成不同步的现象
* CPU也会动态调换顺序

`volatile`可以解决问题一和问题二，它可以：
* 阻止编译器为了提高速度将一个变量缓存到寄存器内而**不写回**
* 阻止编译器调整操作volatile变量的指令顺序

`barrier`指令可以解决问题三：
* CPU的`barrier`指令类似于一个拦水坝，阻止换序"穿透"这个大坝。因此，`barrier`指令之前的语句，一定会先于后面的语句执行

# 第2章 编译和链接
## GCC编译过程
* 预编译 `gcc -E hello.c -o hello.i`
   * 经过预编译后的.i文件不包含任何宏定义，因为所有的宏已经被展开，并且包含的文件也已经被插入到.i文件中。
   * 预编译后会添加行号和文件名标识，比如`#2 "hello.c" 2`，以便于编译时编译器产生调试用的行号信息及用于编译时产生编译错误或警告时能够显示行号。
* 编译 `gcc -S hello.i -o hello.s`
   * 编译过程就是把预处理完的文件进行一系列词法分析、语法分析、语义分析及优化后生产相应的汇编代码文件。
* 汇编 `gcc -c hello.s -o hello.o`
   * 将汇编代码转变成机器指令
   * 比较简单，只是把汇编指令和机器指令一一对应
* 链接 `ld`
   * [Makefile](./code/hello/Makefile)中定义了链接所需要的文件

## 编译器
![compiler](./pictures/compiler.png)
* 词法分析
   * 目的：将源代码分割成一系列的记号(Token)，此时内容不会改变，只做分割
   * 过程：通过扫描器将代码分成如下几种记号：
      * 关键字
      * 标识符
      * 字面量
      * 特殊符号(如加号、等号)
   * `lex`开源工具可以完成这件事
   * 预处理过程不归入编译器，由独立的预处理器完成
* 语法分析
   * 目的：分析Token，产生语法树
   * `yacc`开源工具可以完成语法分析工作
* 语义分析
   * 目的：标识语法树的表达式类型，可能发生类型转换，也可能发生类型报错
   * 过程：分静态语义(编译时确定语义)和动态语义(运行时确定语义，如除0)
   ![./semantic](./pictures/semantic.png)
* 中间语言生成
   * 目的：将语法树优化成机器无关的中间代码
   * 中间代码一般跟目标机器和运行时环境是无关的，比如它不包含数据的尺寸、变量地址和寄存器名字等，常见的形式有:
      * 三地址吗(Three-address Code), 如：`x = y op z`
      * P-代码(P-Code)
   * 达到的效果：中间代码使得编译器可以被分为前端和后端
      * 前端：负责产生机器无关的中间代码
      * 后端：将中间代码转换成目标机器代码
* 目标代码生成与优化
   * 目的：将中间代码转换成机器代码
   * 过程，编译器后端主要包括：
      * 代码生成器(Code Generator)
         * 将中间代码转换成目标机器代码，例如x86的汇编
      * 目标代码优化器(Target Code Optimizer)
         * 对机器代码进行优化，比如选择合适的寻址方式、使用位移来代替乘法运算、删除多余的指令等

## 静态链接
链接过程主要包括了：
* 地址和空间分配(Address and Storage Allocation)
* 符号决议(Symbol Resolution)
* 重定位(Relocation)


# 第3章 目标文件里有什么
## 目标文件的格式
ELF文件分为4类(通过file命令可查看)：
* 可重定位文件(Relocatable File), 如：.o文件
* 可执行文件(Executable File), 如：/bin/bash文件
* 共享目标文件(Shared Object File), 如：.so文件
* 核心转储文件(Core Dump File), 如：core dump
## 目标文件是什么样的
![section](./pictures/section.png)
* 文件头 FileHeader
   * 文件类型
   * 入口地址(如果是可执行文件)
   * 目标硬件
   * 目标操作系统
   * 段表(Section Table), 描述文件中各个段在文件中的偏移位置以及段的属性等
* .text段
   * 机器代码
* .data段
   * 已初始化的全局变量和局部静态变量
* .bss段
   * 未初始化的全局变量和局部静态变量
   * 上述变量的默认值都为零，没有必要存在.data段
   * 只是为未初始化的全局变量和局部静态变量预留位置，并没有内容，所以它在文件中也不占据空间

### 为什么要将代码和数据分开存储
* 保护代码只读，不被篡改
* 共享只读数据

## 挖掘SimpleSection.o
### 分析`objdump -h`的打印内容
```bash
SimpleSection.o:     file format elf32-i386

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .group        00000008  00000000  00000000  00000034  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
  1 .text         00000087  00000000  00000000  0000003c  2**0
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  2 .data         00000008  00000000  00000000  000000c4  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  3 .bss          00000004  00000000  00000000  000000cc  2**2
                  ALLOC
  4 .rodata       00000004  00000000  00000000  000000cc  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .text.__x86.get_pc_thunk.ax 00000004  00000000  00000000  000000d0  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  6 .comment      0000002b  00000000  00000000  000000d4  2**0
                  CONTENTS, READONLY
  7 .note.GNU-stack 00000000  00000000  00000000  000000ff  2**0
                  CONTENTS, READONLY
  8 .note.gnu.property 0000001c  00000000  00000000  00000100  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  9 .eh_frame     0000007c  00000000  00000000  0000011c  2**2
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
```
* "CONTENTS"表示该段在文件中存在
   * BSS段没有"CONTENTS", 表示它实际上在ELF文件中不存在内容
   * .note.GNU-stack段虽然有"CONTENTS", 但是它的长度为0

### 代码段
* `objdump -s`
   * 以十六进制打印各段内容，其长度和文件头中定义的段长度是一致的
* `objdump -d`
   * 打印各段反汇编内容，中间列的十六进制数字和`-s`打印出来的内容一致，代表机器码

### 数据段和只读数据段
```bash
Idx Name          Size      VMA       LMA       File off  Algn
  2 .data         00000008  00000000  00000000  000000c4  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  4 .rodata       00000004  00000000  00000000  000000cc  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA

Contents of section .data:
 0000 54000000 55000000                    T...U...        
Contents of section .rodata:
 0000 25640a00                             %d..            
```
* .data段
   * 保存的是那些已经初始化了的全局静态变量和局部静态变量，此处分别是`global_init_var`与`static_var`，因此长度是8字节(32位系统int是4字节)。
   * 内容也是小端模式(低位存低位)存储的84(0x54)和85(0x55)
* .rodata段
   * 存储只读数据，这里存放的是代码中`printf`用到的字符串常量"%d\n"。ASCII码中，`%`对应0x25，`d`对应的是0x64，`\n`对应的是0x0a，最后是`\0`结尾

### BSS段
```bash
Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  3 .bss          00000004  00000000  00000000  000000cc  2**2
                  ALLOC

SYMBOL TABLE:
00000000 l     O .bss   00000004 static_var2.1513
00000004       O *COM*  00000004 global_uninit_var
```
* 在`SYMBOL_TABLE`中显示，只有`static_var2`在.bss段中，`global_uninit_var`只是一个未定义的"COMMON"符号。这其实和不同的语言与不同的编译器实现有关。有些编译器会将全局的未初始化变量存放在目标文件.bss段，而有些只是预留一个未定义的全局变量符号，等到最终链接成可执行文件的时候再在.bss段分配空间。

### 其他段
![sections](./pictures/sections.png)

### 自定义段
```c
__attribute__((section("FOO"))) int global = 42;
__attribute__((section("BAR"))) void foo()
{
}
```
我们在全局变量或函数之前加上`__attribute__((section("name")))`属性就可以把相应的变量或函数放到以"name"作为段名的段中。

## ELF文件结构描述
![ELF](./pictures/ELF_struct.png)
### 文件头
```
readelf -h SimpleSection.o
ELF Header:
  Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF32
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Intel 80386
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          1140 (bytes into file)
  Flags:                             0x0
  Size of this header:               52 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           40 (bytes)
  Number of section headers:         16
  Section header string table index: 15
```

### 段表 Section Header Table
```
readelf -S SimpleSection.o
There are 16 section headers, starting at offset 0x474:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .group            GROUP           00000000 000034 000008 04     13  17  4
  [ 2] .text             PROGBITS        00000000 00003c 000087 00  AX  0   0  1
  [ 3] .rel.text         REL             00000000 00037c 000048 08   I 13   2  4
  [ 4] .data             PROGBITS        00000000 0000c4 000008 00  WA  0   0  4
  [ 5] .bss              NOBITS          00000000 0000cc 000004 00  WA  0   0  4
  [ 6] .rodata           PROGBITS        00000000 0000cc 000004 00   A  0   0  1
  [ 7] .text.__x86.get_p PROGBITS        00000000 0000d0 000004 00 AXG  0   0  1
  [ 8] .comment          PROGBITS        00000000 0000d4 00002b 01  MS  0   0  1
  [ 9] .note.GNU-stack   PROGBITS        00000000 0000ff 000000 00      0   0  1
  [10] .note.gnu.propert NOTE            00000000 000100 00001c 00   A  0   0  4
  [11] .eh_frame         PROGBITS        00000000 00011c 00007c 00   A  0   0  4
  [12] .rel.eh_frame     REL             00000000 0003c4 000018 08   I 13  11  4
  [13] .symtab           SYMTAB          00000000 000198 000150 10     14  14  4
  [14] .strtab           STRTAB          00000000 0002e8 000092 00      0   0  1
  [15] .shstrtab         STRTAB          00000000 0003dc 000095 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  p (processor specific)
```

### 重定位表 (Relocation Table)
对于每个需要重定位的代码段或数据段，都会有一个想定的重定位表`.rel.xxx`

### 字符串表 (String Table)
常见的字符串表：
   * 字符串表`.strtab`
      * 用来保存普通的字符串，比如符号的名字
   * 段表字符串表`.shstrtab`
      * 用来保存段表中用到的字符串，最常见的就是段名(sh_name)

## 链接的接口--符号
### 常见的符号类型
```bash
> nm SimpleSection.o

00000000 T func1                    # T means the symbol is in the text section
00000000 D global_init_var          # D means the symbol is in the initialized data section
         U _GLOBAL_OFFSET_TABLE_    # U means the symbol is undefined
00000004 C global_uninit_var        # C means the symbol is common
00000032 T main
         U printf
00000004 d static_var.1512
00000000 b static_var2.1513         # b means the symbol is in the BSS data section
00000000 T __x86.get_pc_thunk.ax

> readelf -s SimpleSection.o

Symbol table '.symtab' contains 21 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 00000000     0 FILE    LOCAL  DEFAULT  ABS SimpleSection.c
     2: 00000000     0 SECTION LOCAL  DEFAULT    2 
     3: 00000000     0 SECTION LOCAL  DEFAULT    4 
     4: 00000000     0 SECTION LOCAL  DEFAULT    5 
     5: 00000000     0 SECTION LOCAL  DEFAULT    6 
     6: 00000004     4 OBJECT  LOCAL  DEFAULT    4 static_var.1512
     7: 00000000     4 OBJECT  LOCAL  DEFAULT    5 static_var2.1513
     8: 00000000     0 SECTION LOCAL  DEFAULT    7 
     9: 00000000     0 SECTION LOCAL  DEFAULT    9 
    10: 00000000     0 SECTION LOCAL  DEFAULT   10 
    11: 00000000     0 SECTION LOCAL  DEFAULT   11 
    12: 00000000     0 SECTION LOCAL  DEFAULT    8 
    13: 00000000     0 SECTION LOCAL  DEFAULT    1 
    14: 00000000     4 OBJECT  GLOBAL DEFAULT    4 global_init_var
    15: 00000004     4 OBJECT  GLOBAL DEFAULT  COM global_uninit_var
    16: 00000000    50 FUNC    GLOBAL DEFAULT    2 func1
    17: 00000000     0 FUNC    GLOBAL HIDDEN     7 __x86.get_pc_thunk.ax
    18: 00000000     0 NOTYPE  GLOBAL DEFAULT  UND _GLOBAL_OFFSET_TABLE_
    19: 00000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
    20: 00000032    85 FUNC    GLOBAL DEFAULT    2 main
```
* 定义在本目标文件的全局符号，可以被其他目标文件引用
   * 比如SimpleSection.o里面的`func1`, `main`等
* 外部符号(External Symbol)
   * 在本目标文件中引用的全局符号，却没有定义在本目标文件
   * 比如SimpleSection.o里面的`printf`
* 段名
   * 由编译器产生，值就是该段的起始地址
   * 比如SimpleSection.o里面的`.text`, `.data`等
* 局部符号
   * 这里符号只有在编译单元内部可见
   * 比如SimpleSection.o里面的`static_var`和`static_var2`
   * 调试器可以使用这些符号来分析程序或崩溃时的的核心转储文件，这些局部符号对链接过程没有作用，链接器往往也忽略他们。
* 行号信息
   * 目标文件指令与源代码中代码行的对应关系

### 特殊符号
当我们使用`ld`作为链接器来链接生产可执行文件时，它会为我们定义很多特殊的符号，我们可以直接声明并引用它。
* `__executable_start`
   * 程序起始地址，注意，不是入口地址，是程序的最开始的地址
* `_etext`
   * 代码结束地址
* `_edata`
   * 数据段结束地址
* `_end`
   * 程序结束地址
可参考例子: [special_symbol](./code/special_symbol)

### 符号修饰与函数签名
为了防止符号名冲突，UNIX下的C语言源代码文件中的所有全局变量和函数前会加上下划线`_`。C++为了防止可能的冲突，还增加了命名空间Namespace来解决多模块的符号冲突问题。
* 符号修饰(Name Decoration)和符号改编(Name Mangling)
   * _Z开头
   * N代表namespace
   * E代表End
   * E后面跟参数类型

### extern "C"
* [例子](./code/extern_c)
```bash
> nm main.o

0000000000000000 T func                   # extern C 函数
                 U _GLOBAL_OFFSET_TABLE_
0000000000000025 T main
                 U printf
0000000000000008 D var3
000000000000000c D var4                   # C++ 全局变量
0000000000000012 T _Z4funcf               # C++ 函数
0000000000000000 D _ZN6myname3varE        # C++ namespace里的变量
0000000000000004 D _ZN6myname4var2E
```

### 弱符号与强符号
* 强符号Strong Symbol
   * 多处定义会链接出错
   * 编译器默认函数和初始化了的全局变量为强符号
* 弱符号Weak Symbol
   * 相对于弱符号
   * 未初始化的全局变量为弱符号
   * 也可通过`__attribute__((weak))`来定义任何一个强符号为弱符号
* [例子](./code/weak_strong_symbol)
```bash
> nm symbols.o

0000000000000000 T main
0000000000000000 D strong
0000000000000004 C weak    # C means the symbol is common. Common symbols are uninitialized data.
0000000000000004 V weak2   # V means the symbol is weak object. When a weak defined symbol is linked with a normal defined symbol, the normal defined symbol is used with no error. When a weak undefined symbol is linked and the symbol is not defined, the value of the weak symbol becomes zero with no error.
```
针对强弱符号的概念，链接器就会按如下规则处理于选择被多次定义的全局符号：
* 规则1：不允许强符号被多次定义
* 规则2：如果一个符号在某个目标文件中是强符号，在其他文件中都是弱符号，那么选择强符号
* 规则3：如果一个符号在所有目标文件中都是弱符号，那么选择其中占用空间最大的一个。
#### 强引用与弱引用
* 强引用
   * 对外部目标文件的符号引用，如果没有找到该符号，链接器就会报符号未定义
* 弱引用
   * 没有发现引用符号，也不会报错
   * `__attribute__((weakref))`这个关键字可声明对一个外部函数的引用未弱引用
   * 下面这段代码可成功编译为可执行文件，但是运行时会报错
   ```c
   __attribute__((weakref)) void foo();
   int main()
   {
      foo();
   }
   ```

弱符号和弱引用对于库来说十分有用，比如库中定义的弱符号可以被用户定义的强符号所覆盖，从而使得程序可以使用自定义版本的库函数。或者程序可以扩展功能模块的引用定义为弱引用，当我们将扩展模块与程序链接在一起时，功能模块就可以正常使用。可参考例子[pthread](./code/pthread)。

## 调试信息
如果编译时加上`-g`参数，目标文件里会产生很多`.debug`段。在Linux下，我们可以使用`strip`命令去掉ELF文件中的`.debug`段。

# 第4章 静态链接
链接器采用相似段合并的方法链接目标文件，一般分为两步：
* 空间与地址分配
* 符号解析与重定位

## 空间与地址分配
![link_load](./pictures/link_load.png)

## 符号解析与重定位
* 重定位前
```bash
0000000000000000 <main>:
...
  26:   48 8d 35 00 00 00 00    lea    0x0(%rip),%rsi        # 2d <main+0x2d>
  2d:   48 89 c7                mov    %rax,%rdi
  30:   e8 00 00 00 00          callq  35 <main+0x35>
...
```
* 重定位后
```bash
0000000000401030 <main>:
...
  401056:       48 8d 35 c3 2f 00 00    lea    0x2fc3(%rip),%rsi        # 404020 <shared>
  40105d:       48 89 c7                mov    %rax,%rdi
  401060:       e8 1b 00 00 00          callq  401080 <swap>
...
```
* 链接器是如何知道哪些指令要被调整的？
ELF文件中，有一个重定位表(Relocation Table)的结构专门用来保存这些与重定位相关的信息，它在ELF文件中往往是一个或多个段。
```bash
> objdump -r a.o  # 查看a.o里面要重定位的地方

RELOCATION RECORDS FOR [.text]:
OFFSET           TYPE              VALUE 
0000000000000029 R_X86_64_PC32     shared-0x0000000000000004
0000000000000031 R_X86_64_PLT32    swap-0x0000000000000004
000000000000004a R_X86_64_PLT32    __stack_chk_fail-0x0000000000000004
```
可以发现`0x29`和`0x31`分别是`shared`和`swap`在`.text`段中的位置。

## COMMON块
链接器本身不支持符号的类型，所以当我们定义的多个符号定义类型不一致时，会有三种情况：
* 两个或两个以上强符号类型不一致
   * 链接器报错
* 有一个强符号，其他都是弱符号，出现类型不一致
   * 链接器以强符号为准，如果弱符号空间大于强符号，链接器会警告
* 两个或两个以上弱符号类型不一致
   * 按照COMMON类型的链接规则，以所有弱类型符号占用空间最大的准

### 为什么编译器不直接把未初始化的全局变量也当作未初始化的局部静态变量一样处理，为它在bss段分配空间，而是将其标记为一个COMMON类型的变量？
如果编译单元包含了弱符号，那么该符号最终所占空间的大小在此时是未知的。但是链接器在链接过程中可以确定弱符号的大小，因为当链接器读取所有输入目标文件后，任何一个弱符号的最终大小都可以确定了，所以它可以在最终输出文件的BSS段为其分配空间。所以从总体来看，未初始化全局变量最终还是被放在BSS段的。

## ABI
ABI(ABI Application Binary Interface)，概念类似与API，但是要求更加严格，是二进制层面的接口一致性要求，如两个目标文件能否链接成一个可执行文件。影响ABI的因素非常多：
* 内置类型的大小和存放方式
* 组合类型的存储方式
* 外部符号与用户定义符号的命名方式和解析方式
* 函数调用方式
* 堆栈的分布方式
* 寄存器的使用约定

## 静态库链接
```bash
> ar -t /usr/lib/x86_64-linux-gnu/libc.a

init-first.o
libc-start.o
sysdep.o
version.o
check_fds.o
libc-tls.o
elf-init.o
dso_handle.o
errno.o
errno-loc.o
```

## 链接过程控制
### 不依赖glibc的HelloWorld程序
* [tiny_hello_world](./code/raw_hello)
* 用汇编代码直接触发`0x80`系统调用中断，调用内核API`write`函数，完成print的动作
* `objdump -h main`
   * .text段保存程序指令
   * .rodata保存的是字符串"Hello World!\n"
   * .data保存的是str全局变量
      ```bash
      readelf -S main
      There are 11 section headers, starting at offset 0x3238:

      Section Headers:
      [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
      [ 0]                   NULL            00000000 000000 000000 00      0   0  0
      [ 1] .note.gnu.propert NOTE            08048114 000114 00001c 00   A  0   0  4
      [ 2] .text             PROGBITS        08049000 001000 000071 00  AX  0   0  1
      [ 3] .rodata           PROGBITS        0804a000 002000 00000e 00   A  0   0  1
      [ 4] .eh_frame         PROGBITS        0804a010 002010 000090 00   A  0   0  4
      [ 5] .got.plt          PROGBITS        0804c000 003000 00000c 04  WA  0   0  4
      [ 6] .data             PROGBITS        0804c00c 00300c 000004 00  WA  0   0  4
      [ 7] .comment          PROGBITS        00000000 003010 00002a 01  MS  0   0  1
      [ 8] .symtab           SYMTAB          00000000 00303c 000130 10      9  11  4
      [ 9] .strtab           STRTAB          00000000 00316c 00006c 00      0   0  1
      [10] .shstrtab         STRTAB          00000000 0031d8 00005e 00      0   0  1
      ```
* 使用链接脚本[TinyHelloWorld](./code/raw_hello/TinyHelloWorld.lds)将.text, .data和.rodata段合并成tinytext段，并去除.comment段。
```bash
readelf -S main_tiny
There are 10 section headers, starting at offset 0x3cc:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] tinytext          PROGBITS        008480d4 0000d4 00007b 00 WAX  0   0  1
  [ 2] .text.__x86.get_p PROGBITS        0084814f 00014f 000004 00  AX  0   0  1
  [ 3] .eh_frame         PROGBITS        00848154 000154 000090 00   A  0   0  4
  [ 4] .note.gnu.propert NOTE            008481e4 0001e4 00001c 00   A  0   0  4
  [ 5] .data.rel.local   PROGBITS        00848200 000200 000004 00  WA  0   0  4
  [ 6] .got.plt          PROGBITS        00848204 000204 00000c 04  WA  0   0  4
  [ 7] .symtab           SYMTAB          00000000 000210 0000f0 10      8  10  4
  [ 8] .strtab           STRTAB          00000000 000300 000054 00      0   0  1
  [ 9] .shstrtab         STRTAB          00000000 000354 000076 00      0   0  1
```
* 使用strip去除.symtab和.strtab段
```bash
readelf -S main_strip
There are 8 section headers, starting at offset 0x278:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] tinytext          PROGBITS        008480d4 0000d4 00007b 00 WAX  0   0  1
  [ 2] .text.__x86.get_p PROGBITS        0084814f 00014f 000004 00  AX  0   0  1
  [ 3] .eh_frame         PROGBITS        00848154 000154 000090 00   A  0   0  4
  [ 4] .note.gnu.propert NOTE            008481e4 0001e4 00001c 00   A  0   0  4
  [ 5] .data.rel.local   PROGBITS        00848200 000200 000004 00  WA  0   0  4
  [ 6] .got.plt          PROGBITS        00848204 000204 00000c 04  WA  0   0  4
  [ 7] .shstrtab         STRTAB          00000000 000210 000066 00      0   0  1`
```