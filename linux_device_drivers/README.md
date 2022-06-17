> 《LINUX设备驱动程序》

# 字符设备驱动程序

## scull的设计

## 主设备号和次设备号
```bash
> ls -l /dev
crw-rw-rw-  1 root     root      1,   5 6月   4 22:42 zero
crw-------  1 root     root     10, 249 6月   4 22:42 zfs
```

* 一个主设备号对应一个驱动程序
* 次设备号
    * 用来指向驱动程序所实现的设备
    * 用于正确确定设备文件所指的设备，可以通过次设备号获得一个指向内核设备的直接指针

```cpp
<linux/kdev_t.h>

MAJOR(dev_t dev);
MINOR(dev_t dev);

MKDEV(int major, int minor);
```

### 分配和释放设备编号
```cpp
// 静态分配设备编号
//  first - 要分配的设备编号范围的起始值，常设置为0
//  count - 所请求的连续设备编号的个数
//  name - 是和该编号范围关联的设备名称，将出现在`/proc/devices`和`sysfs`中
int register_chrdev_region(dev_t first, unsigned int count, char *name);

// 动态分配设备编号
//  dev - 输出参数，保存调用完成后分配的第一个编号
//  firstminor - 应该要使用的被请求的第一个次设备号，通常是0
//  count和name - 和上面的函数一致
int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);

// 释放设备编号
void unregister_chrdev_region(dev_t first, unsigned int count);
```

* 上面的函数只是分配了设备编号，接下来要告诉内核这些编号要做什么，将设备编号和内部函数连接起来

### 重要的数据结构

* `file_operations`
    * 提供了文件操作方法，如`open`，`read`，`mmap`等
    * 例如，`scull`设备提供了如下操作：
    ```cpp
    struct file_operations scull_fops = {
        .owner =    THIS_MODULE,
        .llseek =   scull_llseek,
        .read =     scull_read,
        .write =    scull_write,
        .unlocked_ioctl = scull_ioctl,
        .open =     scull_open,
        .release =  scull_release,
    };
    ```

* `file`
    * 系统中每个打开的文件在内核空间中都有一个对应的`file`结构
    * 包括文件模式`f_mode`，读写位置`f_pos`等

* `inode`
    * 对于单个文件，可能会有许多个表示打开的文件描述符的`file`结构，但是它们都指向单个`inode`结构
    * 包括`dev_t i_rdev`(真正的设备编号)，`cdev *i_cdev`(字符设备的内部结构)等

## udev和devfs
> 《Linux设备驱动开发详解》第5.4节

### udevadm工具
* 寻找`/dev/`下节点对应的`/sys`节点
    * `udevadm info -q path -n /dev/tty0`
* 打印udev的规则文件
    * `udevadm info -a -p /sys/devices/virtual/tty/tty0`
    ```bash
    > udevadm info -a -p /sys/devices/virtual/tty/tty0
    Udevadm info starts with the device specified by the devpath and then
    walks up the chain of parent devices. It prints for every device
    found, all possible attributes in the udev rules key format.
    A rule to match, can be composed by the attributes of the device
    and the attributes from one single parent device.

    looking at device '/devices/virtual/tty/tty0':
        KERNEL=="tty0"
        SUBSYSTEM=="tty"
        DRIVER==""
        ATTR{active}=="tty2"
    ```
* `udev`利用内核通过`netlink`套接字发出`uevent`信息，动态创建设备文件节点
