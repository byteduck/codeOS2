void putchar(int c){
	asm volatile("int $0x80" : : "a"(0), "b"(c));
}

void printf(char* c){
	int i = 0;
	while(c[i] != 0){
		putchar(c[i]);
		i++;
	}
}

void syscall(unsigned int callid){
	asm volatile("int $0x80" : : "a"(callid));
}