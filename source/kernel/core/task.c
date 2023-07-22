#include "core/task.h"
#include "tools/klib.h"
#include "os_cfg.h"
#include "cpu/cpu.h"
#include "tools/log.h"


static int tss_init (task_t * task, uint32_t entry, uint32_t esp){
    int tss_sel = gdt_alloc_desc();
    //寻找空闲表项
    if(tss_sel < 0){
        log_printf("alloc tss faied.\n");
        return -1;
    }
    segment_desc_set(tss_sel, (uint32_t)&task->tss, sizeof(tss_t),
        SEG_P_PRESENT | SEG_DPL0 | SEG_TYPE_TSS
    );

     // tss段初始化
    kernel_memset(&task->tss, 0, sizeof(tss_t));
    task->tss.eip = entry;
    task->tss.esp = task->tss.esp0 = esp;
    task->tss.ss0 = KERNEL_SELECTOR_DS;
    task->tss.eip = entry;
    task->tss.eflags = EFLAGS_DEFAULT | EFLAGS_IF;
    task->tss.es = task->tss.ss = task->tss.ds 
            = task->tss.fs = task->tss.gs = KERNEL_SELECTOR_DS;   // 暂时写死
    task->tss.cs = KERNEL_SELECTOR_CS;    // 暂时写死
    task->tss.iomap = 0;

    task->tss_sel = tss_sel;
    return 0;
}

int task_init(task_t * task, uint32_t entry, uint32_t esp){
    ASSERT(task != (task_t *) 0);

    tss_init(task, entry, esp);
    
    return 0;
}
void task_switch_from_to(task_t * from, task_t * to){
    switch_to_tss(to->tss_sel);
}