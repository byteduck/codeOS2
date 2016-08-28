@echo off
cd asm
nasm startup.asm -f elf -o ../bin/startup.o
cd ../c
i686-elf-gcc -ffreestanding -c kernel.c common.c ehci.c fat32.c gdt.c heap.c idt.c irq.c isr.c keyboard.c paging.c pci.c pio.c pit.c print_util_80_25_char.c print_util_base.c shell.c syscall.c tss.c -std=gnu99 -Woverflow
move *.o ../bin
cd ../bin
i686-elf-ld -o disk/codeOS2/system/krnl32.sys startup.o kernel.o common.o ehci.o fat32.o gdt.o heap.o idt.o irq.o isr.o keyboard.o paging.o pci.o pio.o pit.o print_util_80_25_char.o print_util_base.o shell.o syscall.o tss.o -T ../linker.ld
del *.o
xcopy disk D: /s /e /y
pause