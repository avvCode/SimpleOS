#include "cpu/cpu.h"
#include "os_cfg.h"
#include "comm/cpu_instr.h"
static segment_desc_t gdt_table[GDT_TABLE_SIZE];

void segment_desc_set(int selector, uint32_t base, uint32_t limit, uint16_t attr) {
    segment_desc_t * desc = gdt_table + selector  / sizeof(segment_desc_t);

    //limit超过可表示的范围
    if(limit > 0xFFFFF){
        attr |= 0x8000;
        limit /= 0x1000;
    }

    desc->limit15_0 = limit & 0xffff;
	desc->base15_0 = base & 0xffff;
	desc->base23_16 = (base >> 16) & 0xff;
	desc->attr = attr | (((limit >> 16) & 0xf) << 8);
	desc->base31_24 = (base >> 24) & 0xff;
}

void gate_desc_set(gate_desc_t * desc, uint16_t selector, uint32_t offset, uint16_t attr){
    desc->offset15_0 = offset & 0xFFFF;
    desc->selector = selector;
    desc->attr = attr;
    desc->offset31_16 = (offset >> 16) & 0xFFFF;
}

int gdt_alloc_desc(){
    for(int i = 0; i < GDT_TABLE_SIZE; i++){
        segment_desc_t * desc = gdt_table + i;
        if(desc->attr == 0){
            return i * sizeof(segment_desc_t);
        }
    }
    return - 1;
}

void init_gdt (void) {
    for(int i = 0; i < GDT_TABLE_SIZE; i++){
        segment_desc_set( i << 3, 0 , 0 , 0);
    }
    //数据段
    segment_desc_set(KERNEL_SELECTOR_DS, 0, 0xFFFFFFFF,
        SEG_P_PRESENT | SEG_DPL0 | SEG_S_NORMAL | SEG_TYPE_DATA
        | SEG_TYPE_RW | SEG_D
    );
    //代码段
    segment_desc_set(KERNEL_SELECTOR_CS, 0, 0xFFFFFFFF,
        SEG_P_PRESENT | SEG_DPL0 | SEG_S_NORMAL | SEG_TYPE_CODE
        | SEG_TYPE_RW | SEG_D
    );
    //加载GDT
    lgdt((uint32_t) gdt_table, sizeof(gdt_table));
}

void cpu_init(void){
    init_gdt();
}

void switch_to_tss(int tss_sel){
    far_jump(tss_sel, 0);
}