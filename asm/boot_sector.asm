[org 0x7c00]
KERNEL_LOC equ 0x1000 ;Where in memory the kernel will be loaded

mov [BOOT_DRIVE], dl

mov bp, 0x9000 ;Setup stack
mov sp, bp

call kernel_load

mov bx, PM_START_MSG
call print_string
call switch_pm

jmp $

%include 'print_util.asm'
%include 'disk.asm'
%include 'gdt.asm'
%include 'pm.asm'

[bits 16]

kernel_load:
	mov bx, KERNEL_LOAD_MSG
	call print_string
	
	mov dh, 15            ;Load 15 sectors
	mov bx, KERNEL_LOC    ;To KERNEL_LOC
	mov dl, [BOOT_DRIVE]  ;From BOOT_DRIVE
	call disk_load        ;Go!
	
	ret

[bits 32]

pm:
	call KERNEL_LOC
	jmp $           ;Hang if kernel code is finished

BOOT_DRIVE db 0
DISK_ERROR_MSG db "There was an error reading the disk!",0
PM_START_MSG db "Switching to 32-bit mode... ",0
KERNEL_LOAD_MSG db "Loading kernel... ",0

times 510-($-$$) db 0
dw 0xaa55