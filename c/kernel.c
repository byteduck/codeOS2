#include "print_util.h"

int kmain(){
	clearScreen();
	center_print("",0x0f);
	center_print("---------------------------",0x0f);
	center_print("|   Welcome to CodeOS2!   |",0x0f);
	center_print("---------------------------",0x0f);
	center_print("",0x0f);
	println("Successfully booted into 32-bit protected mode");
}