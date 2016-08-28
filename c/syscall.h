#ifndef SYSCALL_H
#define SYSCALL_H

#include "common.h"

struct syscall{
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
};

void syscallHandler(uint32_t eax, uint32_t ebx);

#endif