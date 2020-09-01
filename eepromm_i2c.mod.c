#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x1a93cc1c, "module_layout" },
	{ 0x64c2f661, "i2c_del_driver" },
	{ 0x52a63a23, "i2c_register_driver" },
	{ 0x5eee4b8c, "misc_register" },
	{ 0xf4fa543b, "arm_copy_to_user" },
	{ 0xb70789e, "__might_fault" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0x2819a706, "i2c_transfer_buffer_flags" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x7c32d0f0, "printk" },
	{ 0x3366155, "misc_deregister" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cmse,memIMD");
MODULE_ALIAS("of:N*T*Cmse,memIMDC*");

MODULE_INFO(srcversion, "59232713DEC37D8F7264E4C");
