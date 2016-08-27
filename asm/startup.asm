[global start]
[global load_gdt]
[extern kmain]

KERNEL_VIRTUAL_BASE equ 0xC0000000                  ; 3GB
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)  ; Page directory index of kernel's 4MB PTE.

section .data
align 0x1000
BootPageDirectory:
    dd 0x00000083
    times (KERNEL_PAGE_NUMBER - 1) dd 0
    dd 0x00000083
    times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0
retfromkernel: db 0xa,"|-----------------------|",0xa,"| Returned from kernel. |",0xa,"|-----------------------|"

section .text
align 4
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
    ; NOTE: Until paging is set up, the code must be position-independent and use physical
    ; addresses, not virtual ones!
    mov ecx, (BootPageDirectory - KERNEL_VIRTUAL_BASE)
    mov cr3, ecx                                        ; Load Page Directory Base Register.
 
    mov ecx, cr4
    or ecx, 0x00000010                          ; Set PSE bit in CR4 to enable 4MB pages.
    mov cr4, ecx
 
    mov ecx, cr0
    or ecx, 0x80000000                          ; Set PG bit in CR0 to enable paging.
    mov cr0, ecx
 
    ; Start fetching instructions in kernel space.
    ; Since eip at this point holds the physical address of this command (approximately 0x00100000)
    ; we need to do a long jump to the correct virtual address of StartInHigherHalf which is
    ; approximately 0xC0100000.
    lea ecx, [start_hh]
    jmp ecx                                                     ; NOTE: Must be absolute jump!

start_hh:
	mov dword [BootPageDirectory], 0
    invlpg [0]
	
	mov esp, stack+0x4000
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

section .bss
align 32
stack:
    resb 0x4000      ; reserve 16k stack on a uint64_t boundary