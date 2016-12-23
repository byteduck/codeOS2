#ifndef COMMON_H
#define COMMON_H

#define HIGHER_HALF 0xC0000000
#define true 1
#define false 0
#define NULL ( (void *) 0)

#define __va_argsiz(t)	\
	(((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))


#define va_start(ap, pN)	\
	((ap) = ((va_list) __builtin_next_arg(pN)))



#define va_end(ap)	((void)0)

#define va_arg(ap, t)					\
	 (((ap) = (ap) + __va_argsiz(t)),		\
	  *((t*) (void*) ((ap) - __va_argsiz(t))))

typedef char* va_list;
typedef unsigned int   uint32_t;
typedef          int   sint32_t;
typedef unsigned short uint16_t;
typedef          short sint16_t;
typedef unsigned char  uint8_t;
typedef          char  sint8_t;
typedef char *string;
typedef unsigned char bool;
typedef int int32_t;
typedef uint32_t size_t;
typedef unsigned int uint;

extern uint32_t krnlstart;
extern uint32_t krnlend;

struct registers{
    unsigned int gs, fs, es, dsl;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int num, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};

void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void outl(uint16_t port, uint32_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);
int sgn(int x);
int abs(float x);
void *memset(void *dest, char val, int count);
void *memcpy(void *dest, const void *src, size_t count);
void numToHexString(uint8_t num, char *str);
char nibbleToHexString(uint8_t num);
char *itoa(int i, char *p, int base);
bool isACharacter(uint8_t num);
int strlen(const char *str);
bool strcmp(char *str1, char *str2);
int indexOf(char c, char *str);
int indexOfn(char c, int n, char *str);
void substr(int i, char *src, char *dest);
void substri(int i, char *src, char *dest);
void substrr(int s, int e, char *src, char *dest);
void strcpy(char *src, char *dest);
int countOf(char c, char *str);
bool contains(char *str, char *cont);
void cli();
void sti();

#endif
