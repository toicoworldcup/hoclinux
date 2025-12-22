#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>        
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>   
#include <linux/gpio.h>
#include <linux/of.h>       
#include <linux/of_gpio.h>   

#define DRIVER_NAME "hieu_driver"
#define DEVICE_COUNT 1

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;
static int gpio_red; 
static int gpio_blue; 

//dinh nghia cac ham file op
static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    char command;
    if(copy_from_user(&command, buf, 1)) return -EFAULT;
    
    if(command == 'R') {
        gpio_set_value(gpio_red, 1);
        gpio_set_value(gpio_blue, 0);

    }
    else if (command == 'B') {
        gpio_set_value(gpio_red, 0);
        gpio_set_value(gpio_blue, 1);

    }
    else if (command == 'O') {
        gpio_set_value(gpio_red, 0);
        gpio_set_value(gpio_blue, 0);

    }
    else if (command == 'T') {
        gpio_set_value(gpio_red, 1);
        gpio_set_value(gpio_blue, 1);

    }
    
    return count; 
}
//dang ki file op voi kernel
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = my_write,
};
//khoi tao device file
static int __init chardev_init(void) {
    struct device_node *dn;
    int ret;
    dn = of_find_node_by_path("/hieu_led_node");
    if (!dn) {
        printk(KERN_ERR "hieu_driver: Khong tim thay node trong DT\n");
        return -ENODEV;
    }
    gpio_red = of_get_named_gpio(dn, "gpios", 0);
    if (gpio_red < 0) return gpio_red;
    gpio_blue = of_get_named_gpio(dn, "gpios", 1);
    if (gpio_blue < 0) return gpio_blue;
    alloc_chrdev_region(&dev_num, 0, DEVICE_COUNT, DRIVER_NAME);
    cdev_init(&my_cdev, &fops);
    cdev_add(&my_cdev, dev_num, DEVICE_COUNT);//dang ki character driver
    //tao device file
    my_class = class_create(DRIVER_NAME);
    my_device = device_create(my_class, NULL, dev_num, NULL, DRIVER_NAME);
    //xin cap quyen dieu khien gpio
    gpio_request_one(gpio_red, GPIOF_OUT_INIT_LOW, "red ");
    gpio_request_one(gpio_blue, GPIOF_OUT_INIT_LOW, "blue");

    
    printk(KERN_INFO "hieu_driver: Da lien ket voi GPIO tu Device Tree\n");
    return 0;
}

static void __exit chardev_exit(void) {
    gpio_free(gpio_red);
    gpio_free(gpio_blue);

    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, DEVICE_COUNT);
}

module_init(chardev_init);
module_exit(chardev_exit);
MODULE_LICENSE("GPL");