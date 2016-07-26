section .text
[bits 32]
[global start]
[global load_gdt]
[extern kmain]
	
start:
	push dword ebx
	call kmain
	jmp $
	
load_gdt:
	cli
	lgdt [gdt_descriptor]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:load_gdt2
load_gdt2:
	ret

;%include 'vesa.asm' I still gotta figure out how to deal with this in 32-bit mode
%include 'gdt.asm'
%include 'syscall.asm'
%include 'int.asm'