@echo off
cd asm
nasm startup.asm -f elf -o ../bin/startup.o
nasm multiboot.asm -f elf -o ../bin/multiboot.o
cd ../c
i686-elf-gcc -ffreestanding -c kernel.c -o ../bin/kernel.o -std=gnu99 -Woverflow
cd ../bin
i686-elf-ld -n -o disk/codeOS2/system/KERNEL.SYS multiboot.o startup.o kernel.o -T ../linker.ld
del *.o
xcopy disk E: /s /e /y
pause