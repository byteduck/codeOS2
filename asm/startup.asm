section .text
[bits 32]
[global start]
[global load_gdt]
[extern kmain]
mboot:
	dd  0xe85250d6
	dd  0
	dd  mboot_end-mboot
	dd  -(0xe85250d6 + 0 + (mboot_end - mboot))
	dw 0
	dw 0
	dd 8
mboot_end:
	
start:
	push dword ebx
	call kmain
	mov eax, 1
	mov ebx, retfromkernel
	int 0x80 ;syscall to print string
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
%include 'paging.asm'

section .data
retfromkernel: db "Returned from kernel",0