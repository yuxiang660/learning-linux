# 于渊《ORANGE'S 一个操作系统的实现》读书笔记
# 第1章 环境准备
通过这一章，搭建此文的工作环境，目的是：
* 在VMware的Ubuntu20上，基于Debug Mode的Bochs(2.6.11)，跑起来本书的第一个hello world演示程序。

## 软件版本：
* Ubuntu 20.04.1 LTS on VMware
* Bochs x86 Emulator 2.6.11

## 配置步骤
假设已经安装好Ubuntu20，下面开始编译安装Bochs(2.6.11)。
### 编译安装Bochs
* 下载[bochs-2.6.11.tar.gz](https://sourceforge.net/projects/bochs/files/bochs/)
* 解压并进入主目录: `cd ./bochs-2.6.11`
* 配置工程，开启调试功能：`./configure --enable-debugger --enable-disasm`
* 编译：`make`
   * 如果出现以下错误：`x.cc:37:10: fatal error: X11/Xlib.h: No such file or directory`
   * 安装libx11-dev: `sudo apt-get install libx11-dev`
   * 重新编译：`make dist-clean`, `./configure --enable-debugger --enable-disasm`, `make`
* 安装：`sudo make install`
* 退出"bochs-2.6.11"目录，启动bochs: `cd ~`, `bochs`
   * 出现bochs菜单选项
   ![bochs](./pictures/bochs.png)
* 选择“6. Begin simulation”，输入"c"继续，由于没有指定启动方式，会出现下图：<br>
![bochs2](./pictures/bochs2.png)
* Continue后，得到下图：<br>
![bochs3](./pictures/bochs3.png)

### 启动第一个image
* 准备好image文件"a.img"和bochs配置文件“bochsrc”
   * 文件的准备过程可参考后面的下一章节，这里直接拷贝到[hello_img](./code/hello_img)
* 以配置文件[bochsrc](./code/hello_img/bochsrc)启动bochs：`cd ./code/hello_img`，`bochs -f ./bochsrc`
   * 出现某些文件找不到的错误，请更新配置文件[bochsrc](./code/hello_img/bochsrc)
* 选择“6. Begin simulation”，输入"c"继续后，出现"Hello, OS world!"界面：<br>
![hello_os](./pictures/hello_os.png)
   * 如果出现错误：`>>PANIC<< bochs: cannot connect to X server`，请选择支持"X server"的终端，如：“MobaXterm”，或者直接登录ubuntu的图形界面操作

### 通过VMware启动镜像
* 利用VMware创建一个系统，系统类型选择other，并添加软驱floppy，软驱镜像选择上面生成的a.img，配置如下：
![vmware_hello](./pictures/vmware_hello.png)
* 启动后出现期望界面<br>
![vmware_hello2](./pictures/vmware_hello2.png)

### 通过Qemu启动镜像
* 安装Qemu：`sudo apt update && sudo apt-get install qemu-system`
* 启动镜像：`qemu-system-i386 -fda ./a.img`
   * "-fda file"的意思是：“use 'file' as floppy disk 0/1 image”
* 出现期望的启动界面<br>
![qemu](./pictures/qemu.png)

# 第2章 马上动手写一个最小的“操作系统”
## NASM语法
* 方括号[]的使用
   * 在NASM中，任何不被方括号[]括起来的标签或变量名都被认为是地址，例如：
      ```asm
      foo: dw 1   ; 等价于：foo dw 1
      move ax, foo
      move bx, [foo]
      ```
      * `move ax, foo`是把foo的地址传给ax
      * `move bx, [foo]`是把bx的值赋值为foo的值1
* 关于`$`和`$$`
   * `$`表示当前行被汇编后的地址
   * `$$`表示一个节(section)的开始地址，例子[boot.asm](./code/hello_boot/boot.asm)只有一个节，所以值是0x7c00。
      * `times 510-($-$$) db 0`表示将0这个字节重复510-($-$$)遍，也就是将剩下的空间一直填0，直到510字节。

## 生成hello镜像
所有命令都在[Makefile](./code/hello_boot/Makefile)中:
* `make bin`: 汇编[boot.asm](./code/hello_boot/boot.asm)，生成二进制文件"boot.bin"
   * 打开二进制文件"boot.bin"，总共512(0 ~ 0x1ff)字节，最后两个字节内容是“55 AA”，其他内容是：<br>
   ![boot_bin](./pictures/boot_bin.png)
* `make dis`：反汇编二进制文件“boot.bin”
   ```bash
   # https://stackoverflow.com/questions/14530461/first-os-a-few-assembly-line-explanations
   # nasm boot.asm -o boot.bin
   # ndisasm -o 0x7c00 boot.bin, 起始地址是0x7c00: "org 07c00h"
   00007C00  8CC8              mov ax,cs
   00007C02  8ED8              mov ds,ax
   00007C04  8EC0              mov es,ax
   00007C06  E80200            call 0x7c0b         # 0x7c0b是DispStr的地址
   00007C09  EBFE              jmp short 0x7c09
   00007C0B  B81E7C            mov ax,0x7c1e       # 0x7c1e是字符串“Hello, OS world!”的起始地址
   00007C0E  89C5              mov bp,ax           # ES:BP -> string to write
   00007C10  B91000            mov cx,0x10         # CX = number of characters in string，设置字符串长度0x10
   00007C13  B80113            mov ax,0x1301       # AH = 0x13 表示写字符串，AL = 0x01 表示写模式1
   00007C16  BB0C00            mov bx,0xc          # BH = 0x00 表示page number, BL = 0x0c 表示字符串颜色
   00007C19  B200              mov dl,0x0          # DH,DL = row,column at which to start writing.
   00007C1B  CD10              int 0x10            # http://www.ctyme.com/intr/int-10.htm
   00007C1D  C3                ret
   00007C1E  48                dec ax              # H的ASCII码：0x48
   00007C1F  656C              gs insb             # el
   00007C21  6C                insb                # l
   00007C22  6F                outsw               # o
   00007C23  2C20              sub al,0x20         # , 
   00007C25  4F                dec di              # O
   00007C26  53                push bx             # S
   00007C27  20776F            and [bx+0x6f],dh    #  wo
   00007C2A  726C              jc 0x7c98           # rl
   00007C2C  642100            and [fs:bx+si],ax   # d!
   00007C2F  0000              add [bx+si],al
   ...
   00007DFD  0055AA            add [di-0x56],dl
   ```
* `make img`：生成镜像文件“a.img”
   * “a.img”和“boot.bin”的内容是一样的
* `make bochs`：bochs启动hello界面
* `make qemu`：qemu启动hello界面

## 用Bochs调试操作系统
* Bochs官方调试手册：http://bochs.sourceforge.net/doc/docbook/user/internal-debugger.html
* 常用命令
   * 断点：`b 0x7c00`
   * 继续执行：`c`
   * 查看CPU信息：`info cpu`
   * 查看寄存器：`r`
   * 查看后面Memory的内容(以byte方式，十六进制形式，打印后面30个bytes的memory)：`x /30bx`
   * 单步运行：`s`
   * 开启寄存器跟踪：`trace-reg on`
* Bochs调试命令表:<br>
![bochs_debug4](./pictures/bochs_debug4.png)

### Bochs调试例子
* `r`<br>
![bochs_debug1](./pictures/bochs_debug1.png)
* `x /30bx 0x7c00`<br>
![bochs_debug2](./pictures/bochs_debug2.png)
   * 可以发现上图的内容和[a.img](./code/hello_img/a.img)是一致的
* `trace-reg on`<br>
![bochs_debug3](./pictures/bochs_debug3.png)

# 第3章 保护模式(Protect Mode)

## 认识保护模式

### GDT(Global Descriptor Table)
![descriptor](./pictures/descriptor.png)

GDT的作用是用来提供段式存储机制，这种机制式通过段寄存器和GDT中的描述符共同提供的。上图是代码段和数据段的描述符格式，此外还有系统段描述符和门描述符。

### 如何将cs、ds等段寄存器和GDT中的描述符对应起来？
![selector](./pictures/selector.png)

简单来说，是通过“选择子Selector”将两者联系起来的。选择子的结构如上如，TI和RPL都为零时，选择子就变成了对应描述符想读与GDT基址的偏移(一个描述符8字节)，如同下面例子中的`LABEL_DESC_VIDEO	- LABEL_GDT`。
```nasm
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT
mov	ax, SelectorVideo
mov	gs, ax
```

### 如何将逻辑地址(段：偏移)转化成线性地址？
![address_logical_to_linear](./pictures/address_logical_to_linear.png)

### 如何从实模式到保护模式？
参见代码[“pmtest1”](./code/protect_mode/base/pmtest1.asm)，过程如下：
* 准备好GDT
* 通过指令`lgdt`配置GDTR寄存器，其结构如下：<br>
   ![gdtr](./pictures/gdtr.png)
   * 打开地址线A20
   ```nasm
   ; 打开地址线A20
   in	al, 92h
   or	al, 00000010b
   out	92h, a
   ```
* 配置CR0寄存器使能保护位<br>
   ![cr0](./pictures/cr0.png)
   ```nasm
   ; 准备切换到保护模式
   mov	eax, cr0
   or	eax, 1
   mov	cr0, ea
   ```
* 把代码段的选择子装入cs段寄存器
   ```nasm
   ; 真正进入保护模式
   jmp	dword SelectorCode32:0	; 执行这一句会把 SelectorCode32 装入 cs,
                                 ; 并跳转到 Code32Selector:0  
   ```

### 描述符的属性有哪些位组成？
* P位
   * 表示在段内是否存在
* DPL位
   * 表示特权级，Descriptor Privilege Level
* S位
   * 1：数据段/代码段描述符
   * 0：系统段/门描述符
* TYPE位<br>
   ![descriptor_type](./pictures/descriptor_type.png)
   ![sd_type](./pictures/sd_type.png)
   * 上图中“依从”就是“一致”
* G位
   * 0：段界限粒度为字节
   * 1：段界限粒度为4KB
* D/B位
   * 可执行代码段描述符
      * 1：指令使用32位地址及32位或8位操作数
      * 0：指令使用16位地址及16位或8位操作数
   * 向下扩展数据段描述符
      * 1：段的上部界限位4GB
      * 0：段的上部界限为64KB
   * 堆栈段
      * 1：使用32位堆栈指针寄存器esp
      * 0：使用16位堆栈指针寄存器sp

### 什么是一致码段Conforming Code Segment？
向特权级更高的代码段转移：
* 一致代码段
   * 当前的特权级会被延续
   * 不访问受保护的资源的系统代码和某些类型的异常处理(比如，除法错误或溢出错误)，可以被放在一致代码段中
* 非一致代码段
   * 会引起常规保护错误(general-protection exception, #GP)，除非使用调用门或者任务门
   * 为避免低特权级的程序访问而被保护起来的系统代码应放到非一致代码段中

无论是不是一致代码段，都不能通过call或者jmp，向特权级低的代码段转移，尝试这样的转移将会导致常规保护错误。

所有数据段都是非一致的，这意味着不可能被低特权级的代码访问到。然而，与代码段不同，数据段可以被更高特权级的代码访问到，而不需要使用特定的门。

![call_jmp_rule](./pictures/call_jmp_rule.png)

### 如何在保护模式下访问超过1MB地址空间，并退回到实模式？
参见代码[“pmtest”](./code/protect_mode/enhance/pmtest.asm)，其主要功能是：
* 从实模式进入保护模式
* 在保护模式下从0x0500000线性地址位置读写8字节
* 从保护模式返回实模式

与上节[代码](./code/protect_mode/dos/pmtest1.asm)不同之处在于，此程序在dos下可正常退出，运行结果如下：

![pm_to_rm](./pictures/pm_to_rm.png)

### LDT(Local Descriptor Table)
LDT和GDT区别仅仅在于全局和局部的不同，参加代码["pmtest"](./code/protect_mode/ldt/pmtest.asm)。LDT选择子比GDT选择子多出了一个属性`SA_TIL`，其值是4，对应选择子TI位。如果此位被置位，系统会从LDT中寻找描述符，而不是从GDT中寻找描述符。
```nasm
; LDT 选择子
SelectorLDTCodeA	equ	LABEL_LDT_DESC_CODEA	- LABEL_LDT + SA_TIL
```
创建LDT需要以下额外几个步骤：
* GDT中需要加入LDT的描述符
* 需要用`lldt`指令加载ldtr
   * `lldt`的操作数是GDT中用来描述LDT的描述符
* LDT选择子的TI位要置1
   * 系统发现选择子中的TI位被置位，会从ldtr指向的LDT中寻找对应的描述符

代码执行结果如下，LDT的代码段只是打印一个字符"L":

![ldt_result](./pictures/ldt_result.png)

## 特权级概述
![level](./pictures/level.png)

CPL(Current Privilege Level)
* 存在于cs和ss的第0位和第1位
* 当程序转移到不同特权级的代码时，处理器将改变CPL，一致代码除外

DPL(Descriptor Privilege Level)
* 存在于段或门的描述符中
* DPL将会和CPL以及段或门选择子的RPL相比较，不同类型描述符中的DPL意义不同
   * 数据段：DPL规定了可以访问此段的最低特权级。比如，一个数据段的DPL是1，那么只有运行在CPL为0或者1的程序才有权访问它。
   * 非一致代码段(不使用调用门的情况下)：DPL规定访问此段的特权级。比如，一个非一致代码段的特权级为0，那么只有CPL为0的程序才可以访问它。
   * 调用门：DPL规定了当前执行的程序或任务可以访问此调用门的最低特权级(这与数据段的规则是一致的)。
   * 一致代码段和通过调用门访问的非一致代码段：DPL规定了访问此段的最高特权级。比如，一个一致代码段的DPL是2，那么CPL为0和1的程序将无法访问此段。
   * TSS：DPL规定了可以访问此TSS的最低特权级(这与数据段的规则是一致的)。

RPL(Requested Privilege Level)
* 存在于选择子的第0位和第1位
* 从RPL和CPL中选取特权级低的值起作用
* RPL用来避免低特权级应用程序访问高特权级段内的数据

### 不同特权级代码段之间的转移
程序从一个代码段转移到另一个代码段之前，目标代码段的选择子会被加载到cs中。作为加载过程的一部分，处理器将会检查描述符的界限、类型、特权级等内容。如果检验成功，cs将被加载，程序控制将转移到新的代码段中，从eip指示的位置开始执行。

程序控制转移的发生，可由指令`jmp, call, ret, sysenter, sysexit, int n, iret`引起。使用jmp或call指令可以实现以下几种转移：
* 直接转移
   * 目标操作数包含目标代码段的段选择子
* 间接转移
   * 目标操作数指向一个包含目标代码段选择子的调用门描述符
   * 目标操作数指向一个包含目标代码段选择子的TSS
   * 目标操作数指向一个任务门，这个任务门指向一个包含目标代码段选择子的TSS

### 如何在不同的特权级之间转移？
通过jmp或call的直接转移，
* 对于非一致代码，只能在相同特权级代码之间转移
* 对于一致代码，只能从低特权级到高特权级，且CPL不会改变

如果想自由地进行不同特权级之间转移，需要运用门描述符或者TSS。不同于代码段或数据段的描述符，门描述符的结构如下：<br>
![gate_descriptor](./pictures/gate_descriptor.png)
* 上图中S位是0，代表是门描述符或者系统描述符

一个门描述符由一个选择子和一个偏移所指定的线性地址，程序正是通过这个地址进行转移的。门描述符分别为4种：
* 调用门(Call gates)
* 中断门(Interrupt gates)
* 陷阱门(Trap gates)
* 任务门(Task gates)

代码[call_gate](./code/protect_mode/call_gate/pmtest.asm)中，在GDT中创建了一个调用门描述符，虽然没有特权级的转移，但是阐释了如果通过调用门描述符，转移到目标代码段。
* 在GDT中创建一个调用门描述符
   ```nasm
   LABEL_CALL_GATE_TEST: Gate SelectorCodeDest,   0,     0, DA_386CGate+DA_DPL0
   ```
* 调用门对应的选择子
   ```nasm
   SelectorCallGateTest	equ	LABEL_CALL_GATE_TEST	- LABEL_GDT
   ```
* 通过调用门描述符转移代码
   ```nasm
   ; 测试调用门（无特权级变换），将打印字母 'C'
   call	SelectorCallGateTest:0
   ```

假设我们想由代码A转移到代码B，涉及到：CPL、RPL、代码B的DPL(记做DPL_B)、调用门G的DPL(记做DPL_G)。A访问G这个调用门时，规则相当于访问一个数据段，要求CPL和RPL都小于或者等于DPL_G，总结规则如下：<br>
![call_gate_rule](./pictures/call_gate_rule.png)
* 通过调用门和call指令，可以实现从低特权级到高特权级的转移，无论目标代码段时一致的还是非一致的。

## 短调用和长调用中，调用时和返回时的堆栈是如何的？
* 短调用调用时的堆栈<br>
   ![stack_short_call](./pictures/stack_short_call.png)
* 短调用返回时的堆栈<br>
   ![stack_short_ret](./pictures/stack_short_ret.png)
* 长调用调用时的堆栈<br>
   ![stack_long_call](./pictures/stack_long_call.png)
* 长调用返回时的堆栈<br>
   ![stack_long_ret](./pictures/stack_long_ret.png)

## 当特权级发生变化时，堆栈是如何变化的？
当特权级别变化的时候，堆栈也要发生变化。处理器的这种机制避免了高特权级的过程由于栈空间不足而崩溃。而且，如果不同特权级共享同一个堆栈的话，高特权级的程序可能因此受到有意或无意的干扰。

当发生特权级变化，我们在堆栈A中压入参数和返回时地址，等到需要使用它们的时候堆栈已经变成B了，这该怎么办？Intel提供了这样的一种机制，将堆栈A的诸多内容复制到堆栈B中，如下图。

![stack_privilege_change](./pictures/stack_privilege_change.png)

当发生从低特权级到高特权级转移，处理器从TSS(Task-State Stack)数据结构中获得其余堆栈的ss和esp。并且只有从低特权级到高特权级切换时，新堆栈才会从TS中取得，所以TSS中没有最外层的ring3的堆栈信息。

从低特权级到高特权级转移的过程如下：
* 根据目标代码段的DPL从TSS中选择应该切换至哪个ss和esp
* 从TSS中读取新的ss和esp
* 暂时性地保存当前ss和esp的值
* 加载新的ss和esp
* 将刚刚保存起来的ss和esp的值压入新栈
* 从调用者堆栈中将参数复制到被调用者堆栈(新堆栈)中，复制参数的数目由调用门中Param Count一项来决定。如果Param Count是零的话，将不会复制参数
* 将当前的cs和eip压栈
* 加载调用门中指定的新的cs和eip，开始指向被调用者过程

## 如何从高特权级转移到低特权级？
通过调用门，`call`和`jmp`指令可以实现从低特权级到高特权级的转移，那如何从高特权级转移到低特权级呢？答案是，可以通过`ret`指令。`ret`指令执行前，准备好目标代码段的cs、eip，以及ss和esp，如下图所示：<br>
![stack_ret](./pictures/stack_ret.png)

代码[“ret”](./code/protect_mode/ret/pmtest.asm)通过以下代码，在进入保护模式显示字符“In Protect Mode now.”之后，直接从ring0转移到ring3。
```nasm
push	SelectorStack3
push	TopOfStack3
push	SelectorCodeRing3
push	0
retf
```

执行结果如下：<br>
![ret_results](./pictures/ret_results.png)

## 如何从低特权级转移到高特权级？
调用门和call指令可以使代码从低特权级转移到高特权级，但是由于转移时需要从TSS中获取新的ss和esp，需要通过`ltr`指令加载一个准备好的TSS基地址和大小：
```nasm
mov	ax, SelectorTSS
ltr	ax
```

TSS的结构如下：<br>
![tss](./pictures/tss_structure.png)

参加代码[TSS](./code/protect_mode/tss/pmtest.asm)，运行结果如下：
![tss_result](./pictures/tss_result.png)

从上图中发现，不仅用ring3代码显示了“3”，还通过调用门显示了“C”，代码如下。此程序实现了两次从高特权级到低特权级(retf指令和call调用门)，以及一次从低特权级到高特权级的转移(call调用门)。
```nasm
LABEL_CODE_RING3:
mov	ax, SelectorVideo
mov	gs, ax
mov	edi, (80 * 14 + 0) * 2
mov	ah, 0Ch
mov	al, '3'
mov	[gs:edi], ax

call	SelectorCallGateTest:0

jmp	$
```

## 保护模式总结
代码["final"](./code/protect_mode/final/pmtest.asm)，完成了以下几个过程：
* 构建GDT
   * 其中包括各种描述符：数据描述符、代码描述符、堆栈描述符、LDT描述符、门描述符，TSS描述符等
* 构建LDT
   * 其中的描述符和GDT结构是一样的
* 进入16位实模式
   * 配置正确的段基址到GDT和LDT中，此时就需要创建个描述符对应的代码或数据
   * 通过`lgdt`指令加载GDT
* 进入32位保护模式
   * 显示字符串“In Protect Mode now. ^-^”
   * 通过`ltr`指令加载TSS
   * 通过`retf`指令，从高特权转移到低特权
      * 打印字符“3”
      * 通过调用门，从低特权转移到高特权：`call	SelectorCallGateTest:0`
         * 打印字符“C”
         * 通过`lldt`指令加载LDT
         * 跳入同特权级的局部代码：`jmp	SelectorLDTCodeA:0`
            * 打印字符“L”
            * 跳入同特权级的代码：`jmp	SelectorCode16:0`
               * 关闭保护模式
* 进入16位实模式
   * 正常结束退出

![final_result](./pictures/final_result.png)



## 页式存储

### 分页机制是如何将线性地址转换成物理地址的？
![address_conv](./pictures/address_conv.png)
在未开启分页时，线性地址等于物理地址。开启分页后，线性地址需要通过分页机制转换成物理地址，其过程如下：

![address_linear_to_physical](./pictures/address_linear_to_physical.png)

上图中，页目录大小为4KB，每个表项简称PDE(Page Directory Entry)，每个表项4字节，共有1024各表项。每个页目录的表项PDE对应一个页表，页表的表项简称PTE(Page Table Entry)，共有1024各表项，因此一个页表也是4KB大小。每个页表的表项对应一个物理页，一个物理页的大小也是4KB。

下图是PDE和PTE的结构：

![pde_pte](./pictures/pde_pte.png)
* P位
   * 0：表示页不在内存中，访问将会产生页异常(page-fault exception)
   * 1: 表示存在
* R/W位
   * 0：表示只读
   * 1：表示可读可写
* U/S位
   * 0：表示系统级别
   * 1：表示用户级别

代码[page](./code/page/init/pmtest.asm)的页目录和页表分布如下：<br>
![page_test_mem](./pictures/page_test_mem.png)

我们用4MB的空间来存放页表(1024张页表 * 4KB大小)，它映射了4GB的内存空间。建立页目录和页表的过程如下：
* 初始化一个4KB的页目录，一共1024各表项，每个表项填入页表的地址
* 初始化1024个页表，每个页表里有1024个表项，每个表项填入物理地址的地址
* 将页目录地址填入CR3寄存器
* 使能CR0寄存器

### 如何动态获取内存大小进行分页？
代码[monitor](./code/page/monitor/pmtest.asm)利用了BIOS中断`int 15h`，在实模式下对内存进程了检测。得到了内存大小后，根据内存大小创建大小合适的页表，以防止过大的页表造成内存空间浪费。

![monitor_result](./pictures/monitor_result.png)

### 如何利用分页机制实现线性地址到不同物理地址的转换？
前面的例子虽然创建了页目录和页表，但是线性地址和物理地址都是一样的。分页机制最大的作用是对内存的抽象，用户不需要关心程序运行时的实际物理内存，只需要对线性内存进行操作。因此，下面的代码[switch](./code/page/switch/pmtest.asm)，实现了同一线性地址根据不同的页表，转换成不同的物理地址。

![switch_result](./pictures/switch_result.png)

上图中，`Foo`和`Bar`都是通过调用`call	SelectorFlatC:ProcPagingDemo`打印的，中间做了一次页表切换：
```nasm
call	SelectorFlatC:ProcPagingDemo
call	PSwitch			; 切换页目录，改变地址映射关系
call	SelectorFlatC:ProcPagingDemo
```
