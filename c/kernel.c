#define HIGHER_HALF 0xC0000000

#include "etc/common.h"
#include "etc/multiboot.h"
#include "drivers/VGA/VGA_13h.h"
#include "util/print/print_util_80_25_char.h"
#include "util/draw/draw_util.h"
#include "etc/logo.h"
#include "util/memory/heap.h"
#include "util/memory/paging.h"
#include "util/interrupt/idt.h"
#include "util/interrupt/isr.h"
#include "util/interrupt/irq.h"
#include "util/interrupt/syscall.h"
#include "drivers/disk/pio.h"
#include "drivers/disk/fat32.h"
#include "drivers/keyboard/keyboard.h"
#include "etc/shell.h"
#include "util/interrupt/pit.h"

extern void syscall_handler();
extern void load_gdt();
extern uint8_t boot_disk;
void interrupts_init();
void parse_mboot(uint32_t addr);
int i;

int kmain(uint32_t mbootptr){
	load_gdt();
	interrupts_init();
	setupPaging();
	init_heap();
	parse_mboot(mbootptr+HIGHER_HALF);
	clearScreen();
	center_print("Now in 32-bit protected mode!",0x07);
	//drawMonoBitmap(logo,320,50,0,0,0x2f);
	//ypos = 7;
	uint8_t sect[512];
	readSector(boot_disk, 0, sect);
	if(sect[0] == 0xFF){
		println_color("WARNING: I think you may be booting codeOS2 off of a USB drive or other unsupported device. Disk reading functions may not work.",0x0C);
	}
	if(isPartitionFAT32(boot_disk,getFirstPartition(boot_disk))){
		println("Partition is FAT32!");
	}else{
		println("Partition is not FAT32!");
	}
	fat32part p = getFat32Part(boot_disk,getFirstPartition(boot_disk));
	setCurrentFat32part(p);
	shell();
}

void parse_mboot(uint32_t addr){
	struct multiboot_tag *tag;
	for (tag = (struct multiboot_tag *) (addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))){
		switch (tag->type){
			case MULTIBOOT_TAG_TYPE_BOOTDEV:
			boot_disk = (uint8_t)(((struct multiboot_tag_bootdev *) tag)->biosdev & 0xFF);
			break;
		}
	}
}

void interrupts_init(){
	register_idt();
	isr_init();
	idt_set_gate(0x80, (unsigned)syscall_handler, 0x08, 0x8E);
	irq_add_handler(1, keyboard_handler);
	irq_add_handler(0, pit_handler);
	irq_init();
	asm volatile("sti");
}