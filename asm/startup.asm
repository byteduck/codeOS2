;call init_vesa ;Init VESA VGA

;Set video mode
mov ah, 0x0
mov ah, 0x0
mov al, 0x13
int 0x10

;Enable A20 Gate
mov al, 2
out 0x92, al

call switch_pm

;%include 'vesa.asm' I still gotta figure out how to deal with this in 32-bit mode
%include 'gdt.asm'
%include 'pm.asm'
%include 'syscall.asm'
%include 'int.asm'

[bits 32]
[extern kmain]
pm:
	call kmain
	mov eax,0
	mov ebx,'T'
	int 0x80
	mov eax,1
	int 0x80
	jmp $

PM_START_MSG db "Switching to 32-bit mode...",0x20,0
VESA_LOAD_MSG db "Loading VESA setup...",0x20,0