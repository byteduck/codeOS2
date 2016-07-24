[org 0x7c00]
KERNEL_LOC equ 0x1000 ;Where in memory the kernel will be loaded

jmp main
nop

;FAT16 bootsector stuff

OEM db "CodeOS2 "    ;OEM name
BPS dw 0x200         ;Bytes per sector
SPC db 0x1           ;Sectors per cluster
RS  dw 0x1           ;Reserved Sectors
FC  db 0x2           ;FAT copies
RDE dw 0xe0          ;Root directory entries
SIF dw 0x0           ;Sectors in filesystem (0 for Fat16)
MDT db 0xf0          ;Media descriptor type (F0=Floppy F8=HDD)
SPF dw 0x9           ;Sectors per FAT
SPT dw 0x9           ;Sectors per track
HPC dw 0x2           ;Heads per cylinder
HS  dd 0x0           ;Hidden sectors
NS  dd 0xb40         ;Number of sectors
PDN db 0x80          ;Physical disk number
RES db 0x0           ;Reserved
EXS db 0x29          ;Extended Signature
SN  db "cos2"        ;Serial number of partition
VL  db "CodeOS2Disk" ;Volume Label
FLS db "FAT16   "    ;Filesystem

;Code
main:

mov [PDN], dl

mov bp, 0x9000 ;Setup stack
mov sp, bp

;call vesa_load
;call VESA_LOC

call kernel_load
mov dl,[PDN]
call KERNEL_LOC

jmp $

boot_failed:
mov bx, BOOT_FAIL_MSG
call print_string
jmp $

kernel_load:

	mov dh, 30            ;Load 30 sectors
	mov bx, KERNEL_LOC    ;To KERNEL_LOC
	mov dl, [PDN]         ;From PDN
	mov cl, ROOT_SECT     ;Start from 2nd sector
	call disk_load        ;Go!
	
	ret
	

	
%include 'disk.asm'
%include 'print_util.asm'

BOOT_FAIL_MSG db "Boot failed.",0

times 510-($-$$) db 0
dw 0xaa55