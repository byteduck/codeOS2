#ifndef SHELL_H
#define SHELL_H
#include <ext2.h>

void shell(filesystem_t *fs);
static void command_eval(char *cmd, char *args);

#endif
