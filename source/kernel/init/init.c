/**
 * 内核初始化以及测试代码
 *
 */
#include "comm/boot_info.h"
#include "cpu/cpu.h"
#include "cpu/irq.h"
#include "dev/time.h"
#include "tools/log.h"
#include "os_cfg.h"

/**
 * 内核入口
 */
void kernel_init (boot_info_t * boot_info) {
    //初始化GDT表
    cpu_init();
    log_init();
    irq_init();
    time_init();
}

void init_main(void){
    log_printf("Kernel is running....");
    log_printf("Version: %s", OS_VERSION);
    //int i = 3 / 0;
    //irq_enable_global();
    for(;;){}
}
