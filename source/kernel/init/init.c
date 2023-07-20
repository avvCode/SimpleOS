/**
 * 内核初始化以及测试代码
 *
 */
#include "comm/boot_info.h"
#include "cpu/cpu.h"
#include "cpu/irq.h"
#include "dev/time.h"

/**
 * 内核入口
 */
void kernel_init (boot_info_t * boot_info) {
    //初始化GDT表
    cpu_init();
    irq_init();
    time_init();
}

void init_main(void){
    //int i = 3 / 0;
    //irq_enable_global();
    for(;;){}
}
