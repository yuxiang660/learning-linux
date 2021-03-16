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

### 如何从实模式到保护模式？
参见代码[pmtest1](./code/protect_mode/pmtest1.asm)
