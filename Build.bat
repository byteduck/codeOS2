@echo off
cd asm
nasm startup.asm -f elf -o ../tmp/startup.o
cd ../src
i686-elf-gcc -ffreestanding -c kernel/*.c -I include/kernel -std=gnu99 -Woverflow
move *.o ../tmp 1>NUL 2>NUL
cd ../tmp
i686-elf-ld -o ../out/disk/codeOS2/system/krnl32.sys *.o -T ../linker.ld
del *.o
cd ../out
xcopy disk D: /s /e /y 1>NUL 2>NUL
pause