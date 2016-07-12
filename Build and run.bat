@echo off
cd asm
nasm boot_sector.asm -f bin -o ../bin/boot_sector.bin
pause
qemu-system-i386 ../bin/boot_sector.bin
del trace-*