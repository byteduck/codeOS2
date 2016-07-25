uint16_t PCIReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
 }

uint16_t getPCIVendor(uint8_t bus, uint8_t slot, uint8_t function){
	uint16_t vendor, device;
	vendor = PCIReadWord(bus, slot, function, 0);
	if(vendor != 0xFFFF){
		//The device exists if the vendor is not 0xFFFF
		device = PCIReadWord(bus, slot, 0, 2);
		//Then we can do other stuff here
	}
	return vendor;
}

void PCIDebug(){
	println("Checking all PCI buses...");
	for(int bus = 0; bus < 256; bus++){
		for(int device = 0; device < 32; device++){
			if(getPCIVendor(bus, device, 0) != 0xFFFF){
				print("Found ");
				uint16_t classAndSub = PCIReadWord(bus, device, 0, 10);
				printPCIClassCode((uint8_t)(classAndSub >> 8), (uint8_t)classAndSub, (uint8_t)(PCIReadWord(bus, device, 0, 8) >> 8));
				print(" at ");
				printHex(bus);
				print(",");
				printHex(device);
				if(((uint8_t)(PCIReadWord(bus,device,0,14) & 0xFF)) & 0x80 != 0){
					print(" With Functions ");
					for(uint8_t func = 0; func < 8; func++){
						if(getPCIVendor(bus,device,func) != 0xFFFF){
							classAndSub = PCIReadWord(bus, device, func, 10);
							printPCIClassCode((uint8_t)(classAndSub >> 8), (uint8_t)classAndSub, (uint8_t)(PCIReadWord(bus, device, func, 8) >> 8));
							print(",");
						}
					}
				}
				println("");
			}
		}
	}
	println("Done!");
}

void printPCIClassCode(uint8_t classCode, uint8_t subClass, uint8_t progIF){
	switch(classCode){
		case 0x1:
		print("Mass Storage Controller");
		break;
		case 0x2:
		print("Network Controller");
		break;
		case 0x3:
		print("Display Controller");
		break;
		case 0x04:
		print("Multimedia Controller");
		break;
		case 0x05:
		print("Memory Controller");
		break;
		case 0x06:
		print("Bridge Device");
		break;
		case 0x07:
		print("Simple Communication Controller");
		break;
		case 0x08:
		print("Base System Peripheral");
		break;
		case 0x09:
		print("Input Device");
		break;
		case 0x0A:
		print("Docking Station");
		break;
		case 0x0B:
		print("Processor");
		break;
		case 0x0c:
		print("Serial Bus Controller");
		break;
		case 0x0D:
		print("Wireless Controller");
		break;
		case 0x0E:
		print("Intelligent IO Controller");
		break;
		case 0x0F:
		print("Satellite Communication Controller");
		break;
		case 0x10:
		print("Encryption Controller");
		break;
		case 0x11:
		print("Data Acquisition and Signal Processing Controller");
		break;
		default:
		print("Other Device (");
		printHex(classCode);
		print(")");
		break;
	}
	print(" (Sub ");
	printHex(subClass);
	print(",");
	printHex(progIF);
	print(")");
}