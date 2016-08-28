#ifndef GDT_H
#define GDT_H

#include "tss.h"
#include "common.h"

typedef struct GDTEntry{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed)) GDTEntry;

typedef struct GDTPointer{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) GDTPointer;

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

void load_gdt();

#endif