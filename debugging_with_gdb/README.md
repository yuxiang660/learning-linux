# Chapter2 Getting In & Out of GDB
## 常见的GDB启动方式
* gdb program
   * [例子](./code/hello)
* gdb program core
   * [例子](./code/core)
* gdb program 1234
   * 这样可以attach GDB到进程1234 (除非同时有一个文件叫1234，GDB会首先检查是否有一个core文件)
## 选择文件


