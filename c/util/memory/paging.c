uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t page_tables[1024] __attribute__((aligned(4096)));
uint8_t ebuf[256] __attribute__((aligned(4096)));
uint32_t krnlstartPhys = (uint32_t)&krnlstart-HIGHER_HALF;
uint32_t krnlendPhys = (uint32_t)&krnlend-HIGHER_HALF;
extern uint32_t BootPageDirectory;

void setupPaging(){
	uint32_t i,j;
	page_directory[0]= 0x83;
	for(i = 1; i < (HIGHER_HALF >> 22); i++){
		page_directory[i] = 0x0;
	}
	page_directory[i] = 0x83;
	i++;
	for(i=i; i < 1024; i++){
		page_directory[i] = 0;
	}
	uint32_t *d = (uint32_t*)0x1000;
	load_page_dir((uint32_t *)((uint32_t)&page_directory[0]-HIGHER_HALF));
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
	println("");
	if(other){
		print("Error Code: ");
		printHex(r->err_code);
		println("");
	}
	while(true);
}