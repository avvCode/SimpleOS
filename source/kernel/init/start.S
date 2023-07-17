/**
 *
 * 二级引导，负责进行硬件检测，进入保护模式，然后加载内核，并跳转至内核运行
 */
 #include "os_cfg.h"
  	// 不必加.code32因默认就是32位
 	.text
 	.global _start
	.extern kernel_init
    .extern init_main
_start:
    # 第一种方法
    # push %ebp
    # mov %esp, %ebp
    # mov 0x8(%ebp), %eax
    # push %eax

    # 第二种方法
    # mov 4(%esp), %eax
    # push %eax

    # 第三种方法
    push 4(%esp)

    # kernel_init(boot_info)
    call kernel_init

    jmp $KERNEI_SELECTOR_CS , $gdt_reload
gdt_reload:
    mov $KERNEI_SELECTOR_DS, %ax		// 16为数据段选择子
	mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov $(stack + KERNEL_STACK_SIZE) , %esp

	jmp init_main

    .bss

.comm stack, KERNEL_STACK_SIZE


