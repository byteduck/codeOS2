#include "etc/common.h"
#include "drivers/VGA/VGA_13h.h"
#include "util/print/print_util_320_200_256.h"
#include "util/draw/draw_util.h"
#include "etc/logo.h"

int kmain(){
	clearScreen();
	println("");println("");println("");println("");println("");println("");println("");
	//drawLine(10,10,310,10,0x0f);
	center_print("Now in 32-bit protected mode!",0x07);
	drawMonoBitmap(logo,320,50,0,0,0x2f);
}