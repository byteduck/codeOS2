struct syscall{
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
};

void syscallHandler(uint32_t eax, uint32_t ebx);

#include "syscall.c"