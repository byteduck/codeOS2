i686-elf-gcc -c testexecutable.c -o exec.o -ffreestanding -std=gnu99
i686-elf-ld exec.o -o exec --oformat binary -emain -Ttext 0x0
pause