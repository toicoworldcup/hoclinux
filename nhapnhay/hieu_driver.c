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
#include <linux/timer.h> 

#define DRIVER_NAME "hieu_driver" //ten driver
#define DEVICE_COUNT 1 //so luong device

static dev_t dev_num; //bien luu so hieu thiet bi
static struct timer_list my_timer; //bien timer
static struct cdev my_cdev;//bien cdev
static struct class *my_class;  //bien class
static struct device *my_device;// bien device
static int gpio_red = 0 ; //bien luu so hieu gpio
static int gpio_blue = 0; 
static unsigned long period_a = 500; //thoi gian nap nhap nhay (ms)
static int timer_running = 0;//bien kiem tra trang thai timer
static int blink_count = 0;//bien dem so lan nhap nhay
//ham xu ly timer
void my_timer_callback(struct timer_list *t) {
    blink_count++;
    gpio_set_value(gpio_red, !gpio_get_value(gpio_red));//dao trang thai led do
    if(blink_count%2==0){
         gpio_set_value(gpio_blue, !gpio_get_value(gpio_blue));
    }
    if(timer_running){
        mod_timer(&my_timer, jiffies + msecs_to_jiffies(period_a));//nap lai timer
    }
}
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
        timer_running = 0;
        del_timer(&my_timer);//xoa timer neu no dang chay
        gpio_set_value(gpio_red, 0);
        gpio_set_value(gpio_blue, 0);

    }
    else if (command == 'T') {
        gpio_set_value(gpio_red, 1);
        gpio_set_value(gpio_blue, 1);

    }
    else if(command == 'H'){
        timer_running = 1;
        blink_count = 0;
        mod_timer(&my_timer, jiffies + msecs_to_jiffies(period_a));
    }
    
    return count; 
}

static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    char message[64];
    int len;
    int red_sta = gpio_get_value(gpio_red);
    int blue_sta = gpio_get_value(gpio_blue);
    if (*ppos > 0) return 0; //chi doc 1 lan
    len = sprintf(message,"red led : %s,blue red: %s\n",
                (red_sta?"ON":"OFF"),
                (blue_sta)?"ON":"OFF");//tao chuoi trang thai led va lay do dai
    if(copy_to_user(buf,message,len)){
        return -EFAULT;
    }
    *ppos += len;//cap nhat offset
    
    return len; 
}

//dang ki file op voi kernel
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = my_write,
    .read = my_read,
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
    timer_setup(&my_timer, my_timer_callback, 0);
   
    //xin cap quyen dieu khien gpio t
    gpio_request_one(gpio_red, GPIOF_OUT_INIT_LOW, "red ");
    gpio_request_one(gpio_blue, GPIOF_OUT_INIT_LOW, "blue");

    
    printk(KERN_INFO "hieu_driver: Da lien ket voi GPIO tu Device Tree\n");
    return 0;
}
//huy device file
static void __exit chardev_exit(void) {
    gpio_free(gpio_red);
    gpio_free(gpio_blue);
    del_timer(&my_timer);
    timer_running = 0;

    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, DEVICE_COUNT);
}

module_init(chardev_init);
module_exit(chardev_exit);
MODULE_LICENSE("GPL");