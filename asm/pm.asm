[bits 16]
switch_pm: ;Switch to 32-bit protected mode

	cli

	lgdt [gdt_descriptor] ;Load gdt

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	jmp KERNEL_CODE_SEG:pm_init

[bits 32]

pm_init:
	
	mov ax, KERNEL_DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000
	mov esp, ebp
	call pm