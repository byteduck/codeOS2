@echo off
cd asm
nasm boot_sector.asm -f bin -o ../bin/boot_sector.bin
nasm kernel_load.asm -f elf -o ../bin/kernel_load.o
cd ../c
i686-elf-gcc -ffreestanding -c kernel.c -o ../bin/kernel.o -std=gnu99
cd ../bin
i686-elf-ld -o kernel.bin -Ttext 0x1000 kernel_load.o kernel.o --oformat binary --entry kmain
type boot_sector.bin kernel.bin > os_image.bin
pause
qemu-system-i386 ../bin/os_image.bin
del trace-*