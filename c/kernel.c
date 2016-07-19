#include "print_util_320_200_256.h"
#include "common.h"

int kmain(){
	clearScreen();
	center_print("",0x0f);
	center_print("---------------------------",0x0f);
	center_print("|   Welcome to CodeOS2!   |",0x0f);
	center_print("---------------------------",0x0f);
	center_print("",0x0f);
	center_print("Now in 32-bit protected mode!",0x07);
}