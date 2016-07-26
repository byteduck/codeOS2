typedef struct EHCIController{
	PCIDevice pciDevice;
	uint8_t flags;
} EHCIController;

EHCIController getFirstEHCIController();
void EHCIDebug();
#include "ehci.c"