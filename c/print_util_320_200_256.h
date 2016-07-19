#include "font_8x8.h"
void putch_color(char c, char color);
void print_color(char* c, char color);
void println_color(char* c, char color);
void putch(char c);
void print(char* c);
void println(char* c);
void clearScreen();
void setAllColor(char color);
void setColor(char color);
void center_print(char* c, char color);
#include "print_util_320_200_256.c"