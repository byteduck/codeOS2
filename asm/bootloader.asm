[org 0x7c00]
KERNEL_LOC equ 0x1000 ;Where in memory the kernel will be loaded

mov [BOOT_DRIVE], dl

mov bp, 0x9000 ;Setup stack
mov sp, bp

;call vesa_load
;call VESA_LOC

call kernel_load
mov dl,[BOOT_DRIVE]
call KERNEL_LOC

jmp $

kernel_load:

	mov dh, 50            ;Load 50 sectors
	mov bx, KERNEL_LOC    ;To KERNEL_LOC
	mov dl, [BOOT_DRIVE]  ;From BOOT_DRIVE
	mov cl, 0x02          ;Start from 2nd sector
	call disk_load        ;Go!
	
	ret
	
%include 'disk.asm'

times 218-($-$$) db 0 ;Pad for disk time stamp
;MBR
TIMESTAMP times 8 db 0
BOOT_DRIVE db 0
PART_TBL_OFFSET dw 0

times 436-($-$$) db 0

DISK_UID times 10 db 0
PART1 times 16 db 0
PART2 times 16 db 0
PART3 times 16 db 0
PART4 times 16 db 0

dw 0xaa55