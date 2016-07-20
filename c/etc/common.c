void outb(u16int port, u8int value){
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port){
   u8int ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

u16int inw(u16int port){
   u16int ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

int sgn(int x){
	if(x>0) return 1;
	else return -1;
	return 0;
}

int abs(float x){
	return (int)x;
}