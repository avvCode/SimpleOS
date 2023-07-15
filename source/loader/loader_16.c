__asm__(".code16gcc");

#include "loader.h"


boot_info_t boot_info;


/**
 * BIOS下显示字符串
 */
static void show_msg (const char * msg) {
    char c;

	// 使用bios写显存，持续往下写
	while ((c = *msg++) != '\0') {
		__asm__ __volatile__(
				"mov $0xe, %%ah\n\t"
				"mov %[ch], %%al\n\t"
				"int $0x10"::[ch]"r"(c));
	}
}

static void detect_memory(void){
	show_msg("try to detect memory:");

	uint32_t contID = 0;

	SMAP_entry_t smap_entry;
	int signature , bytes;
	boot_info.ram_region_count = 0;
	for(int i = 0; i < BOOT_RAM_REGION_MAX; i++){
		SMAP_entry_t *entry = &smap_entry;
		__asm__ __volatile__(
			"int  $0x15"
			: "=a"(signature), "=c"(bytes), "=b"(contID)
			: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(entry)
		);
		if(signature != 0x534D4150){
			show_msg("failed.\r\n");
			return;
		}
		if(bytes > 20 && (entry->ACPI & 0x0001) == 0){
			continue;
		}
		if(entry->Type == 1){
			boot_info.ram_region_cfg[boot_info.ram_region_count].start = entry->BaseL;
			boot_info.ram_region_cfg[boot_info.ram_region_count].size = entry->LengthL;
			boot_info.ram_region_count++;
		}
		if(contID == 0){
			break;
		}
	}
	show_msg("ok.\r\n");
}

//gdt表
uint16_t gdt_table[][4] = {
	{0,0,0,0},
	{0xFFFF,0x0000,0x9A00,0x00CF},
	{0xFFFF,0x0000,0x9200,0x00CF},
};

//进入保护模式
static void enter_protect_mode(void) {
	cli(); //关中断
	//打开A20地址线，不开启无法访问4GB内存
	uint8_t v = inb(0x92);
	outb(0x92, v | 0x2);
	//加载gdt表
	lgdt((uint32_t)gdt_table,sizeof(gdt_table));

	//读取cr0
	uint32_t cr0 = read_cr0();
	write_cr0(cr0 | (1 << 0));
	//跳转
	far_jump(8,(uint32_t)protect_mode_entry);
}


void loader_entry(void) {
    show_msg("....loading.....\n\r"); //调用BIOS中断显示字符串
	detect_memory(); // 调用BIOS中断查看内存
	enter_protect_mode();// 进入保护模式
    for(;;) {}
}