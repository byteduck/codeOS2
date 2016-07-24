[global mboot]

section .multiboot_header
mboot:
	dd  0xe85250d6
	dd  0
	dd  mboot_end-mboot
	dd  -(0xe85250d6 + 0 + (mboot_end - mboot))
	dw 0
	dw 0
	dd 8
mboot_end: