#ifndef VGA_13H_H
#define VGA_13H_H

int VGA_WIDTH = 320;
int VGA_HEIGHT = 200;
char VGA_MODE = 0x13;
void putPixel(int x, int y, char color);
void setPalette(char id, char r, char g, char b);

#endif