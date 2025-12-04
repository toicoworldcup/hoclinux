#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>       
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#define DRIVER_NAME "hieu_driver"
#define DEVICE_COUNT 1
#define LED_GPIO 23
//khai bao luu major minor number
static dev_t dev_num;
//khai bao struct cdev
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;
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
    char command;
    int value;
    printk(KERN_INFO "%s: Device write operation called. Count: %zu\n", DRIVER_NAME, count);
    if(count > 0){
        if(copy_from_user(&command,buf,1)){
            return -EFAULT;
        }
        if(command == '1'){
            value =1;//high
            printk(KERN_INFO "%s : '1' is received.Led on\n",DRIVER_NAME);
        } else if (command == '0'){
            value=0;//low
            printk(KERN_INFO"%s command '0' is received.led off\n",DRIVER_NAME);
        } else {
            printk(KERN_WARNING "command unvailble \n");
            return -EINVAL;
        }
        gpio_set_value(LED_GPIO,value);
    }
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

    //dang ki driver character
    ret = cdev_add(&my_cdev, dev_num, DEVICE_COUNT);
    if (ret < 0) {
        printk(KERN_ERR "%s: Failed to add cdev.\n", DRIVER_NAME);
        unregister_chrdev_region(dev_num, DEVICE_COUNT);
        return ret;
    }
    //tao device file
    my_class = class_create(DRIVER_NAME);
    if (IS_ERR(my_class)) {
        printk(KERN_ERR "%s: Failed to create device class.\n", DRIVER_NAME);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, DEVICE_COUNT);
        return PTR_ERR(my_class);
    }
    my_device = device_create(my_class, NULL, dev_num, NULL, DRIVER_NAME);
    if (IS_ERR(my_device)) {
        printk(KERN_ERR "%s: Failed to create device.\n", DRIVER_NAME);
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, DEVICE_COUNT);
        return PTR_ERR(my_device);
    }
    //cau hinh ougtput
    ret = gpio_request_one(LED_GPIO,GPIOF_OUT_INIT_LOW,DRIVER_NAME);
    if (ret <0){
        printk(KERN_ERR "%s:Failed to request GPIO \n",DRIVER_NAME);
        device_destroy(my_class,dev_num);
        class_destroy(my_class);
        cdev_del(&my_cdev);  
        unregister_chrdev_region(dev_num, DEVICE_COUNT);
        return ret;
    }
    
    printk(KERN_INFO "%s: Module initialized successfully.\n", DRIVER_NAME);
    return 0;
}
//xoa device file
static void __exit chardev_exit(void)
{
    gpio_free(LED_GPIO);
    printk(KERN_INFO "%s : GPIO %d free",DRIVER_NAME,LED_GPIO);
    device_destroy(my_class,dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    
    unregister_chrdev_region(dev_num, DEVICE_COUNT);
    
    printk(KERN_INFO "%s: Module exited successfully. Device numbers released.\n", DRIVER_NAME);
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A basic Character Driver example.");

