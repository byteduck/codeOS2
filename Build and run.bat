@echo off
cd asm
nasm startup.asm -f elf -o ../bin/startup.o
cd ../c
i686-elf-gcc -ffreestanding -c kernel.c -o ../bin/kernel.o -std=gnu99 -Woverflow
cd ../bin
i686-elf-ld -o KERNEL.SYS -Ttext 0x1000 startup.o kernel.o -T ../linker.ld --oformat binary --entry kmain
del *.o
del trace-*
pause