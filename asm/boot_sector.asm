[org 0x7c00] ;Tell BIOS where to load this in memory to give the stack more space
mov dx, 0x0fab
call print_hex
jmp $

%include 'print_util.asm'

times 510 -( $ - $$ ) db 0
dw 0xaa55