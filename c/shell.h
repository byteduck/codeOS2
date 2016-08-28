#ifndef SHELL_H
#define SHELL_H

#include "keyboard.h"
#include "fat32.h"
#include "common.h"

void shell();
static void command_eval(char *cmd, char *args);

#endif