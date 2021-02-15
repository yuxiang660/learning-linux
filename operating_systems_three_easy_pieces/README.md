# 配置环境
## 安装“XV6”系统
* Ubuntu下安装qemu
   > sudo apt update && sudo apt-get install qemu-system
   * 安装完成后需要支持`qemu-system-i386`命令，否则会出现`Error: Couldn't find a working QEMU executable.`错误。可自行搜索相关依赖
* 安装xv6
   > git clone git://github.com/mit-pdos/xv6-public.git
   > make
* 启动xv6
   > make qemu
   * 在xv6目录下输入上面的命令，即可出现如下xv6窗口：
   ![xv6](./pictures/xv6.png)

