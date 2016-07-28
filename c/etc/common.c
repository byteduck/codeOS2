#define true 1
#define false 0

void outb(uint16_t port, uint8_t value){
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

void outw(uint16_t port, uint16_t value){
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

void outl(uint16_t port, uint32_t value){
    asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port){
   uint8_t ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

uint16_t inw(uint16_t port){
   uint16_t ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

uint32_t inl(uint16_t port){
   uint32_t ret;
   asm volatile ("inl %1, %0" : "=a" (ret) : "dN" (port));
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

void *memset(void *dest, char val, int count){
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

void *memcpy(void *dest, const void *src, int count){
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void numToHexString(uint8_t num, char *str){
	str[1] = nibbleToHexString(num);
	str[0] = nibbleToHexString(num >> 4);
}

char nibbleToHexString(uint8_t num){
	uint8_t tmp = num & 0xF;
	if(tmp < 0xA){
		return tmp+0x30;
	}else{
		return tmp+0x37;
	}
}