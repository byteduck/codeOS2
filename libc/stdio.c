void putchar(int c){
	asm("movl %%ebx, %0;" : "=r" (c) : );
	syscall(0);
}

void printf(char* c){
	int i = 0;
	while(c[i] != 0){
		putchar(c[i]);
	}
}

void syscall(unsigned int callid){
	asm("movl %%eax, %0;" : "=r" (callid) : );
	asm volatile ("int $0x80");
}