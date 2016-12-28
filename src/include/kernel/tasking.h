#ifndef TASKING_H
#define TASKING_H

#include <common.h>

#define PROCESS_ALIVE 0
#define PROCESS_ZOMBIE 1
#define PROCESS_DEAD 2

typedef struct process_t{
	char *name;
	uint32_t pid;
	uint32_t esp;
	uint32_t stack;
	uint32_t eip;
	uint32_t cr3;
	uint32_t state;
	struct process_t *next, *prev;
} process_t;

void initTasking();
process_t *createProcess(char *name,  uint32_t loc);
void preempt();

extern void _iret();

#endif
