#ifndef EHCI_H
#define EHCI_H

#include "pci.h"
#include "common.h"

typedef struct EHCIController{
	PCIDevice pciDevice;
	uint8_t flags;
} EHCIController;

EHCIController getFirstEHCIController();
void EHCIDebug();

#endif