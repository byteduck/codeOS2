#include <common.h>
#include <stdio.h>
#include <tasking.h>
#include <syscall.h>

void syscallHandler(uint32_t eax, uint32_t ebx){
	switch(eax){
		case 0:
			;char c = (char)ebx;
			putch(c);
			break;
		case 1:
			;char* c2 = (char*)ebx;
			print(c2);
			break;
		case 2:
			__kill__();
			break;
		default:
			print("Other syscall ");
			printHex(eax);
			println("");
			break;
	}
}
