@echo off
cd asm
nasm main.asm -f bin -o ../bin/main.bin
pause
qemu-system-i386 ../bin/main.bin
del trace-*