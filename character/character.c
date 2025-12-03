#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>       
#include <linux/cdev.h>
#define DRIVER_NAME "hieu_driver"
#define DEVICE_COUNT 1
//khai bao luu major minor number
static dev_t dev_num;
//khai bao struct cdev
static struct cdev my_cdev;
//dinh nghia file operations
static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "%s: Device opened successfully.\n", DRIVER_NAME);
    return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    printk(KERN_INFO "%s: Device read operation called. Count: %zu\n", DRIVER_NAME, count);
    return 0; 
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    printk(KERN_INFO "%s: Device write operation called. Count: %zu\n", DRIVER_NAME, count);
    return count; 
}

static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "%s: Device closed.\n", DRIVER_NAME);
    return 0;
}

//dang ki operations
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_release,

};

//khoi tao device file
static int __init chardev_init(void)
{
    int ret;
    
    ret = alloc_chrdev_region(&dev_num, 0, DEVICE_COUNT, DRIVER_NAME);
    if (ret < 0) {
        printk(KERN_ERR "%s: Failed to allocate device number.\n", DRIVER_NAME);
        return ret;
    }

    printk(KERN_INFO "%s: Registered with Major: %d, Minor: %d\n", DRIVER_NAME, MAJOR(dev_num), MINOR(dev_num));

    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;

    ret = cdev_add(&my_cdev, dev_num, DEVICE_COUNT);
    if (ret < 0) {
        printk(KERN_ERR "%s: Failed to add cdev.\n", DRIVER_NAME);
        unregister_chrdev_region(dev_num, DEVICE_COUNT);
        return ret;
    }
    
    printk(KERN_INFO "%s: Module initialized successfully.\n", DRIVER_NAME);
    return 0;
}
//xoa device file
static void __exit chardev_exit(void)
{
    cdev_del(&my_cdev);
    
    unregister_chrdev_region(dev_num, DEVICE_COUNT);
    
    printk(KERN_INFO "%s: Module exited successfully. Device numbers released.\n", DRIVER_NAME);
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A basic Character Driver example.");

