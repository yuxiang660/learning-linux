#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static char *who_m = "world";
static int how_many = 2;
module_param(how_many, int, S_IRUGO | S_IWUSR);
module_param(who_m, charp, S_IRUGO | S_IWUSR);

static int __init hello_init(void)
{
	int i = 0;
	for (i = 0; i < how_many; i++)
	{
		printk(KERN_INFO "'%s' enter\n", who_m);
	}
	return 0;
}
module_init(hello_init);

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Hello World exit\n ");
}
module_exit(hello_exit);

MODULE_AUTHOR("Barry Song <21cnbao@gmail.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple Hello World Module");
MODULE_ALIAS("a simplest module");
