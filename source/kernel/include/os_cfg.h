#ifndef OS_CFG_H
#define OS_CFG_H

#define GDT_TABLE_SIZE 256
#define KERNEI_SELECTOR_CS (1*8)  //第一个表项偏移地址
#define KERNEI_SELECTOR_DS (2*8)
#define KERNEL_STACK_SIZE (8*1024)
#endif