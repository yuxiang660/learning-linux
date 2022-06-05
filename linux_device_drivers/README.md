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
