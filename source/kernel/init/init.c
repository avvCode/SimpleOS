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
#include "core/task.h"

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

static task_t first_task;
static uint32_t init_task_stack[1024];
static task_t init_task;

void init_task_entry(void){
    int count = 0;
    for(;;){
        log_printf("init task: %d", count++);
    }
}

void init_main(void){
    log_printf("Kernel is running....");
    log_printf("Version: %s, name: %s", OS_VERSION, "tiny x86 os");
    log_printf("%d %d %x %c", -123, 123456, 0x12345, 'a');
    
    task_init(&init_task, (uint32_t)init_task_entry, &init_task_stack[1024]);
    task_init(&first_task, 0 , 0);

    int count = 0;
    for(;;){
        log_printf("init main: %d", count++);
    }
    init_task_entry();
}
