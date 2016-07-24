section .text
[bits 32]
[global start]
[extern kmain]

start:
	cli
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

PM_START_MSG db "Switching to 32-bit mode...",0x20,0
VESA_LOAD_MSG db "Loading VESA setup...",0x20,0
BOOT_DRIVE dd 0