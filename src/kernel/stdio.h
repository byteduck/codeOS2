void print_color(char* c, char color);
void println_color(char* c, char color);
void putch(char c);
void print(char* c);
void println(char* c);
void setColor(char color);
void center_print_base(char* c, char color, int width);
void printHex(uint8_t num);
void printHexw(uint16_t num);
void printHexl(uint32_t num);
void backspace();
void PANIC(char *error, char *msg, bool hang);
#include "print_util_80_25_char.h"
#include "stdio.c"