# 第0章 环境准备
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

# 第1章 马上动手写一个最小的“操作系统”
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


