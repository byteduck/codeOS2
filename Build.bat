@echo off
cd asm
nasm startup.asm -f elf -o ../bin/startup.o
cd ../src/kernel
i686-elf-gcc -O2 -ffreestanding -c kmain.c -o ../../../bin/kmain.o -std=gnu99 -Woverflow
cd ../../../bin
i686-elf-ld -O2 -o disk/codeOS2/system/krnl32.sys startup.o kmain.o -T ../linker.ld
del *.o
xcopy disk D: /s /e /y
pause