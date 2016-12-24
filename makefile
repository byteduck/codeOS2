SRCS = $(wildcard src/kernel/*.c)
CFILES = $(patsubst %.c,%.o,$(SRCS))
INC = -Isrc/include/kernel
CFLAGS = -ffreestanding -std=gnu99 -Woverflow -m32
FLAGS =
CC = gcc
LD = ld

codeos2: $(CFILES)
	mv src/kernel/*.o .
	nasm asm/startup.asm -f elf -o startup.o
	ld -o out/disk/boot/codek32 *.o -T linker.ld -melf_i386
	rm *.o
	./copy.sh
	qemu-system-i386 -drive file=/dev/loop0,cache=directsync,format=raw

%.o: %.c
	$(CC) $(FLAGS) $(INC) $(CFLAGS) -c -o $@ $<
