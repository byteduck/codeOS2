uint16_t PCIReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint16_t getPCIVendor(uint8_t bus, uint8_t slot, uint8_t function);
void PCIDebug();
void printPCIClassCode(uint8_t classCode, uint8_t subClass, uint8_t progIF);
#include "pci.c"