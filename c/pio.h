#ifndef PIO_H
#define PIO_H

#include "common.h"

void readSector(int disk, int address, uint8_t *sect);
void prepareDisk(int disk, int address);
int getFSType(int disk);
int getFirstPartition(int disk);

#endif