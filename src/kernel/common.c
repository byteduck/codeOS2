#include <common.h>
#include <common.h>

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

void *memcpy(void *dest, const void *src, size_t count){
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

bool isACharacter(uint8_t num){
	return num >= 0x20 && num <= 0x7E;
}

int strlen(const char *str){
        const char *s;

        for (s = str; *s; ++s)
                ;
        return (s - str);
}

bool strcmp(string str1,string str2){
    int i = 0;
	bool flag = false;
   
    while(str1[i]!='\0' && str2[i]!='\0'){
         if(str1[i]!=str2[i]){
             flag=1;
             break;
         }
         i++;
    }

    return flag == 0 && str1[i] == '\0' && str2[i] == '\0';
	
}

int indexOf(char c, char *str){
	int i = 0;
	while(str[i] != '\0'){
		if(str[i] == c)
			return i;
		i++;
	}
	return strlen(str);
}

int indexOfn(char c, int n, char *str){ //like indexOf, except ignores n instances of the character
	int i = 0;
	int count = 0;
	while(str[i] != '\0'){
		if(str[i] == c)
			if(count == n)
				return i;
			else
				count++;
		i++;
	}
	return strlen(str);
}

void substr(int i, char *src, char *dest){ //substring exclusive
	memcpy(dest,src,i);
	dest[i] = '\0';
}

void substri(int i, char *src, char *dest){ //substring inclusive
	memcpy(dest,src,i+1);
	dest[i+1] = '\0';
}

void substrr(int s, int e, char *src, char *dest){ //substring exclusive range (end is exclusive, beginning is inclusive)
	memcpy(dest,&src[s],e-s);
	dest[e-s] = '\0';
}

void strcpy(char *src, char *dest){
	memcpy(dest, src, strlen(src));
	dest[strlen(src)] = '\0';
}

int countOf(char c, char *str){ //Returns number of instances of c in str
	int count = 0;
	for(int i = 0; i < strlen(str); i++){
		if(str[i] == c)
			count++;
	}
	return count;
}

bool contains(char *str, char *cont){ //Returns true if str has cont in it.
	int i = 0;
	int contlen = strlen(cont);
	bool flaga = false;
	bool flagb = false;
	while(str[i+contlen-1] != '\0'){
		flagb = true;
		for(int j = 0; j < strlen(cont); j++){
			if(cont[j] != str[j+i])
				flagb = false;
		}
		if(flagb)
			flaga = true;
		i++;
	}
	return flaga;
}

void cli(){
	asm volatile("cli");
}

void sti(){
	asm volatile("sti");
}