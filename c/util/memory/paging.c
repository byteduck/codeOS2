uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
void setupPaging(){
	int i;
	for(i = 0; i < 1024; i++){
		page_directory[i] = 0x00000002;
	}
	for(i = 0; i < 1024; i++){
		first_page_table[i] = (i * 0x1000) | 3;
	}
	printHexl(first_page_table[0]);
	page_directory[0] = ((unsigned int)first_page_table) | 3;
	load_page_dir(page_directory);
}

void pageFaultHandler(struct registers *r){
	cli();
	unsigned int err_pos;
	asm volatile ("mov %%cr2, %0" : "=r" (err_pos));
	PANIC("PAGE_FAULT", "Occurred at:", false);
	printHexl(err_pos);
	while(true);
}