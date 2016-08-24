@echo off
cd asm
nasm startup.asm -f elf -o ../bin/startup.o
cd ../c
i686-elf-gcc -ffreestanding -c kernel.c -o ../bin/kernel.o -std=gnu99 -Woverflow
cd ../bin
i686-elf-ld -o disk/codeOS2/system/KERNEL.SYS startup.o kernel.o -T ../linker.ld
del *.o
xcopy disk D: /s /e /y
pause