typedef unsigned int   uint32_t;
typedef          int   sint32_t;
typedef unsigned short uint16_t;
typedef          short sint16_t;
typedef unsigned char  uint8_t;
typedef          char  sint8_t;
typedef char *string;
typedef unsigned char bool;

void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void outl(uint16_t port, uint32_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);
int sgn(int x);
int abs(float x);
void *memset(void *dest, char val, int count);
void numToHexString(uint8_t num, char *str);
char nibbleToHexString(uint8_t num);

#include "common.c"