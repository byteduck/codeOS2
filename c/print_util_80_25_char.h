#ifndef PRINT_UTIL_80_25_CHAR_H
#define PRINT_UTIL_80_25_CHAR_H

#include "common.h"
#include "print_util_base.h"

void putch_color(char c, char color);
void clearScreen();
void setAllColor(char color);
void center_print(char* c, char color);
void update_cursor();
void scroll();
void PANIC_base(char *error, char *msg);
#define SCREEN_CHAR_WIDTH 80
#define SCREEN_CHAR_HEIGHT 25

#endif