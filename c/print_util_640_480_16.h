#ifndef PRINT_UTIL_640_480_16_H
#define PRINT_UTIL_640_480_16_H

#include "font_8x8.h"
#include "print_util_base.h"
#include "VGA_13h.h"
void putch_color(char c, char color);
void clearScreen();
void center_print(char* c, char color);

#endif