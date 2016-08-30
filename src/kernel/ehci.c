#include <common.h>
#include <heap.h>
#include <stdio.h>
#include <pci.h>
#include <ehci.h>

EHCIController getFirstEHCIController(){
	PCIDevice device = getPCIDevice(0x0C,0x03,0x20);
	EHCIController controller = {device,0};
	return controller;
}

void EHCIDebug(){
	println("Trying to find USB 2.0 controller(s)...");
	EHCIController controller = getFirstEHCIController();
	PCIDevice device = controller.pciDevice;
	if(device.flags & 0b00000001){
		print("Found USB 2.0 controller at ");
		printHex(device.bus);
		print(",");
		printHex(device.slot);
		println("!");
		if(device.flags & 0b00000010){
			println("There are multiple.");
		}
	}else{
		println("No USB 2.0 controller found.");
	}
}