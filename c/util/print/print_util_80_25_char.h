void putch_color(char c, char color);
void clearScreen();
void setAllColor(char color);
void center_print(char* c, char color);
void update_cursor();
void scroll();
#include "print_util_base.h"
#include "print_util_80_25_char.c"