uint8_t boot_disk = 0;

void readSector(uint8_t disk, uint32_t address, uint8_t *sect){
	outb(0x1F1,0x00);
	outb(0x1F2,0x01);
	outb(0x1F3,(uint8_t)address);
	outb(0x1F4,(uint8_t)(address >> 8));
	outb(0x1F5,(uint8_t)(address >> 16));
	outb(0x1F6,0xE0 | (disk << 4) | ((address >> 24) & 0x0F));
	outb(0x1F7,0x20);
	while (!(inb(0x1F7) & 0x08)) {}
	for(int i = 0; i < 256; i++){
		uint16_t tmp = inw(0x1F0);
		sect[i*2] = (uint8_t)tmp;
		sect[i*2+1] = (uint8_t)(tmp >> 8);
	}
}