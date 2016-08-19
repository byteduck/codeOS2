section .text
[bits 32]
[global start]
[global load_gdt]
[extern kmain]
	
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
	
retfromkernel: db "Returned from kernel"

;%include 'vesa.asm' I still gotta figure out how to deal with this in 32-bit mode
%include 'gdt.asm'
%include 'syscall.asm'
%include 'int.asm'
%include 'paging.asm'
%include 'misc.asm'