#ifndef SHELL_H
#define SHELL_H
#include <ext2.h>

void shell(ext2_partition *ext2);
static void command_eval(char *cmd, char *args);

#endif
