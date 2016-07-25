section .text
[bits 32]
[global start]
[global BOOT_DRIVE]
[extern kmain]

start:
	cli
	mov [BOOT_DRIVE], dl
	lgdt [gdt_descriptor]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:start2
start2:
	call kmain
	jmp $

;%include 'vesa.asm' I still gotta figure out how to deal with this in 32-bit mode
%include 'gdt.asm'
%include 'syscall.asm'
%include 'int.asm'

BOOT_DRIVE: db 0