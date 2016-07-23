void syscallHandler(uint32_t eax, uint32_t ebx){
	switch(eax){
		case 0:
			;char c = (char)ebx; //There is a ; at the beginning because in C there cannot be declarations after labels
			putch(c);
			break;
		default:
			println("Other syscall");
			break;
	}
}