typedef struct PCIDevice{
	uint8_t bus;
	uint8_t slot;
	uint8_t flags; // Bit 0: exists Bit 1: Multiple devices with this device's class, subclass, and progIF
} PCIDevice;
uint16_t PCIReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint16_t getPCIVendor(uint8_t bus, uint8_t slot, uint8_t function);
PCIDevice getPCIDevice(uint8_t class, uint8_t subClass, uint8_t progIF);
void PCIDebug();
void printPCIClassCode(uint8_t classCode, uint8_t subClass, uint8_t progIF);
#include "pci.c"