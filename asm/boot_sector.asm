[org 0x7c00]
KERNEL_LOC equ 0x1000 ;Where in memory the kernel will be loaded

mov [BOOT_DRIVE], dl

mov bp, 0x9000 ;Setup stack
mov sp, bp

;call vesa_load
;call VESA_LOC

call kernel_load

mov bx, PM_START_MSG
call print_string

;call init_vesa ;Init VESA VGA

;Set video mode
mov ah, 0x0
mov ah, 0x0
mov al, 0x13
int 0x10

call switch_pm

jmp $

;%include 'vesa.asm' I still gotta figure out how to deal with this in 32-bit mode
%include 'print_util.asm'
%include 'disk.asm'
%include 'gdt.asm'
%include 'pm.asm'

[bits 16]

kernel_load:
	mov bx, KERNEL_LOAD_MSG
	call print_string
	
	mov dh, 30            ;Load 30 sectors
	mov bx, KERNEL_LOC    ;To KERNEL_LOC
	mov dl, [BOOT_DRIVE]  ;From BOOT_DRIVE
	mov cl, 0x02          ;Start from 2nd sector
	call disk_load        ;Go!
	
	ret

[bits 32]

pm:
	call KERNEL_LOC
	jmp $           ;Hang if kernel code is finished

BOOT_DRIVE db 0
DISK_ERROR_MSG db "There was an error reading the disk!",0x20,0
PM_START_MSG db "Switching to 32-bit mode...",0x20,0
KERNEL_LOAD_MSG db "Loading kernel...",0x20,0
VESA_LOAD_MSG db "Loading VESA setup...",0x20,0

times 510-($-$$) db 0
dw 0xaa55