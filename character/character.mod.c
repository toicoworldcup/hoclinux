#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x4c1dbbd9, "cdev_add" },
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0x0c72f9ad, "cdev_del" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0xefd5d5d8, "cdev_init" },
	{ 0xba157484, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x4c1dbbd9,
	0x0bc5fb0d,
	0x0c72f9ad,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0x9f222e1e,
	0xefd5d5d8,
	0xba157484,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"cdev_add\0"
	"unregister_chrdev_region\0"
	"cdev_del\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"alloc_chrdev_region\0"
	"cdev_init\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "10424639A62BBCB074CD7CF");
