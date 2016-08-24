uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t page_tables[5][1024] __attribute__((aligned(4096)));
uint8_t ebuf[256] __attribute__((aligned(4096)));

void setupPaging(){
	uint32_t i,j;
	for(i = 0; i < 1024; i++){
		page_directory[i] = 0x3;
	}
	for(i = 0; i < 5; i++){
		for(j = 0; i < 1024; i++)
			page_tables[j][i] = (i * 0x1000+(0x400000*j)) | 3;
	}
	/*j = 0;
	for(i = (uint32_t)&kstart/0x1000; i < (uint32_t)&kend/0x1000; i++){
		first_page_table[j] = (i * 0x1000) | 3;         //Mapping our kernel to 0x0
		j++;
	}*/
	for(j = 0; j < 5; j++)
		page_directory[j] = ((unsigned int)page_tables[j]) | 3;
	load_page_dir(page_directory);
}

void pageFaultHandler(struct registers *r){
	cli();
	unsigned int err_pos;
	asm volatile ("mov %%cr2, %0" : "=r" (err_pos));
	bool other = false;
	switch(r->err_code){
		case 0:
		case 1:
			PANIC("KRNL_READ_NONPAGED_AREA", "Occurred at:", false);
			break;
		case 2:
		case 3:
			PANIC("KRNL_WRITE_NONPAGED_AREA", "Occurred at:", false);
			break;
		case 4:
		case 5:
			PANIC("USR_READ_NONPAGED_AREA", "Occurred at:", false);
			break;
		case 6:
		case 7:
			PANIC("USR_WRITE_NONPAGED_AREA", "Occurred at:", false);
			break;
		default:
			PANIC("UNKNOWN_PAGE_FAULT", "Occurred at:", false);
			other = true;
			break;
	}
	
	printHexl(err_pos);
	if(other){
		println("");
		print("Error Code: ");
		printHex(r->err_code);
	}
	while(true);
}