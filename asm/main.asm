[org 0x7c00] ;Tell BIOS where to load this in memory to give the stack more space

mov [MAIN_DRIVE], dl ;The BIOS automatically stores the boot drive in dl

mov bp, 0x8000
mov sp, bp ;Give stack lots of room

mov bx, 0x9000       ;We didn't set ES, so we are loading to 0:0x9000
mov dh, 1            ;1 Sector
mov dl, [MAIN_DRIVE] ;Main Drive
call disk_load

mov bx, 0x9000
call print_string ;Should print "Hello World!"

jmp $

%include 'print_util.asm'
%include 'disk.asm'

MAIN_DRIVE: db 0

times 510 -( $ - $$ ) db 0
dw 0xaa55

HELLO_WORLD:
	db "Hello World!"