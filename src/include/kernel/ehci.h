#ifndef EHCI_H
#define EHCI_H

typedef struct EHCIController{
	PCIDevice pciDevice;
	uint8_t flags;
} EHCIController;

EHCIController getFirstEHCIController();
void EHCIDebug();

#endif