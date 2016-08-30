extern void syscall_handler();
extern void load_gdt();
extern uint8_t boot_disk;
void interrupts_init();
void parse_mboot(uint32_t addr);

int kmain(uint32_t mbootptr);