const char *fat32sig = "FAT32   ";
bool isPartitionFAT32(int disk, int sect){
	uint8_t buf[512];
	readSector(disk, sect, buf);
	bool flag = true;
	for(int i = 0; i < 8; i++){
		if(buf[0x52+i] != fat32sig[i])
			flag = false;
	}
	return flag;
}