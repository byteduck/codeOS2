MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[bits 32]

[global mboot]
[extern code]
[extern bss]
[extern end]

mboot:
	dd  MBOOT_HEADER_MAGIC
	dd  MBOOT_HEADER_FLAGS
	dd  MBOOT_CHECKSUM
	dd  mboot
	dd  code
	dd  bss
	dd  end
	dd  start

[global start]
[extern kmain]

start:
	push ebx
	cli
	lgdt [gdt_descriptor]
	call kmain
	jmp $

;%include 'vesa.asm' I still gotta figure out how to deal with this in 32-bit mode
%include 'gdt.asm'
%include 'syscall.asm'
%include 'int.asm'

PM_START_MSG db "Switching to 32-bit mode...",0x20,0
VESA_LOAD_MSG db "Loading VESA setup...",0x20,0
BOOT_DRIVE dd 0