nasm boot_sector.asm -f bin -o boot_sector.bin
pause
qemu-system-i386 boot_sector.bin
del trace-*