i686-elf-gcc -c testexecutable.c -o exec.o -ffrestanding -std=gnu99
i686-elf-ld exec.o -o exec --oformat binary -emain
pause