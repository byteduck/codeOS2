#include <tasking.h>
#include <heap.h>
#include <common.h>
#include <stdio.h>
#include <pit.h>

//big thanks to levOS (levex on GitHub) for some bits of the tasking code

process_t *current;
process_t *kernel;
uint32_t cpid = 0;
bool wait = false;
bool tasking_enabled = false;

void kthread(){
	tasking_enabled = true;
	wef:
	putch('d');
	goto wef;
}

void test(){
	teee:
	wait = true;
	putch('e');
	goto teee;
}

process_t *createProcess(char *name, uint32_t loc){
	process_t *p = kmalloc(sizeof(process_t));
	memset(p,0,sizeof(process_t)); //make sure everything's clean

	p->name = name;
	p->pid = ++cpid;
	p->state = PROCESS_ALIVE;
     p->eip = loc;
     p->esp = (uint32_t) kmalloc(4096);
     asm volatile("mov %%cr3, %%eax":"=a"(p->cr3));
	uint32_t *stack = (uint32_t *)(p->esp + 4096);
	p->stack = p->esp;

	//pushing registers on to the stack
	*--stack = 0x202; // eflags
	*--stack = 0x8; // cs
	*--stack = loc; // eip
	*--stack = 0; // eax
	*--stack = 0; // ebx
	*--stack = 0; // ecx;
	*--stack = 0; //edx
	*--stack = 0; //esi
	*--stack = 0; //edi
	*--stack = p->esp + 4096; //ebp
	*--stack = 0x10; // ds
	*--stack = 0x10; // fs
	*--stack = 0x10; // es
	*--stack = 0x10; // gs

	p->esp = (uint32_t)stack;
	return p;
}

void __init__(){
	asm volatile("mov %%eax, %%esp": :"a"(current->esp));
	asm volatile("pop %gs");
	asm volatile("pop %fs");
	asm volatile("pop %es");
	asm volatile("pop %ds");
	asm volatile("pop %ebp");
	asm volatile("pop %edi");
	asm volatile("pop %esi");
	asm volatile("pop %edx");
	asm volatile("pop %ecx");
	asm volatile("pop %ebx");
	asm volatile("pop %eax");
	asm volatile("iret");
}

void initTasking(){
	kernel = createProcess("codek32", (uint32_t)kthread);
	kernel->next = createProcess("test", (uint32_t)test);
	kernel->next->next = kernel;
	kernel->prev = kernel;
	current = kernel;

	__init__();
	//pop all of the registers off of the stack and get started
	PANIC("Failed to init tasking", "Something went wrong..", true);
}

uint32_t popval;

void preempt(){
	//push current process' registers on to its stack
	asm volatile("push %eax");
	asm volatile("push %ebx");
	asm volatile("push %ecx");
	asm volatile("push %edx");
	asm volatile("push %esi");
	asm volatile("push %edi");
	asm volatile("push %ebp");
	asm volatile("push %ds");
	asm volatile("push %es");
	asm volatile("push %fs");
	asm volatile("push %gs");
	asm volatile("mov %%esp, %%eax":"=a"(current->esp));
	current = current->next;
	//pop all of next process' registers off of its stack
	asm volatile("mov %%eax, %%cr3": :"a"(current->cr3));
	asm volatile("mov %%eax, %%esp": :"a"(current->esp));
	asm volatile("pop %gs");
	asm volatile("pop %fs");
	asm volatile("pop %es");
	asm volatile("pop %ds");
	asm volatile("pop %ebp");
	asm volatile("pop %edi");
	asm volatile("pop %esi");
	asm volatile("out %%al, %%dx": :"d"(0x20), "a"(0x20));
	asm volatile("pop %edx");
	asm volatile("pop %ecx");
	asm volatile("pop %ebx");
	asm volatile("pop %eax");
	//while(wait);
	asm volatile("iret");
}
