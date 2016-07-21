#include "etc/common.h"
#include "drivers/VGA/VGA_13h.h"
#include "util/print/print_util_320_200_256.h"
#include "util/draw/draw_util.h"
#include "etc/logo.h"
#include "util/interrupt/idt.h"
#include "util/interrupt/isr.h"
#include "util/interrupt/irq.h"
#include "util/syscall.h"

extern void syscall_handler();

int kmain(){
	register_idt();
	isr_init();
	idt_set_gate(0x80, (unsigned)syscall_handler, 0x08, 0x8E);
	irq_init();
	asm volatile("sti");
	clearScreen();
	println("");println("");println("");println("");println("");println("");println("");
	//drawLine(10,10,310,10,0x0f);
	center_print("Now in 32-bit protected mode!",0x07);
	drawMonoBitmap(logo,320,50,0,0,0x2f);
}