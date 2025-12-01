#include <linux/module.h> 
#include <linux/kernel.h>  
 
// 1. HÀM KHỞI TẠO (Entry Point)
static int __init hello_init(void) 
{
    // printk: Hàm in log vào Kernel buffer
    printk(KERN_INFO "--- [HELLO] Module da duoc nap vao Kernel! ---\n");
    
    // Bắt buộc phải có: Trả về 0 báo hiệu thành công.
    return 0; 
}

// 2. HÀM KẾT THÚC (Exit Point)
static void __exit hello_exit(void)
{
    printk(KERN_INFO "--- [HELLO] Module da duoc go bo khoi Kernel! ---\n");
}

// 3. ĐĂNG KÝ HÀM
// Sử dụng các macro chuẩn của Linux để đăng ký Entry và Exit Point
module_init(hello_init);
module_exit(hello_exit);

// 4. THÔNG TIN BẮT BUỘC KHÁC
// MODULE_LICENSE là bắt buộc.
MODULE_LICENSE("GPL"); 
// MODULE_DESCRIPTION là nên có để mô tả module của bạn.
MODULE_DESCRIPTION("A simple Hello World Kernel Module."); 
// MODULE_AUTHOR (Tùy chọn)
MODULE_AUTHOR("Hieu");