gdt_start:

gdt_null:
	dd 0x0
	dd 0x0

gdt_kernel_code:
	dw 0xffff    ;Limit
	dw 0x0       ;Base bits 0-15
	db 0x0       ;Base bits 16-24
	db 10011010b ;Access Flags (Code, Ring 0)
	db 11001111b ;Granularity Flags (4K Granularity)
	db 0x0       ;Base bits 25-31

gdt_kernel_data:
	dw 0xffff    ;Limit
	dw 0x0       ;Base bits 0-15
	db 0x0       ;Base bits 16-24
	db 10010010b ;Access Flags (Data, Ring 0)
	db 11001111b ;Granularity Flags (4K granularity)
	db 0x0       ;Base bits 25-31

gdt_user_code:
	dw 0xffff    ;Limit
	dw 0x0       ;Base bits 0-15
	db 0x0       ;Base bits 16-24
	db 11111010b ;Access Flags (Code, Ring 3)
	db 11001111b ;Granularity Flags (4K granularity)
	db 0x0       ;Base bits 25-31

gdt_user_data:
	dw 0xffff    ;Limit
	dw 0x0       ;Base bits 0-15
	db 0x0       ;Base bits 16-24
	db 11110010b ;Access Flags (Data, Ring 3)
	db 11001111b ;Granularity Flags (4K granularity)
	db 0x0       ;Base bits 25-31
	
gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

KERNEL_CODE_SEG equ gdt_kernel_code - gdt_start
KERNEL_DATA_SEG equ gdt_kernel_data - gdt_start
USER_CODE_SEG equ gdt_user_code - gdt_start
USER_DATA_SEG equ gdt_user_data - gdt_start

;TODO make a userspace memory area
;TODO make a driverspace memory area