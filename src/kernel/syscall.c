#include <common.h>
#include <stdio.h>
#include <syscall.h>

void syscallHandler(uint32_t eax, uint32_t ebx){
	switch(eax){
		case 0:
			;char c = (char)ebx; //There is a ; at the beginning because in C there cannot be declarations after labels
			putch(c);
			break;
		case 1:
			;char* c2 = (char*)ebx;
			print(c2);
			break;
		default:
			print("Other syscall ");
			printHex(eax);
			println("");
			break;
	}
}