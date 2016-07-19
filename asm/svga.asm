init_svga:
	pusha
	push es
	push di
	mov ax,0xdd
	mov es,ax
	mov di,0
	call svga_test
	mov ax,0x4f01
	mov cx,0x0112 ;640*480*24 mode
	int 0x10
	mov dx, [es:di]
	call print_hex
	cmp ax,0x004f
	je svga_supported
	jmp svga_err
	popa
	ret

svga_test:
	mov ax,0x4f01
	int 0x10
	cmp ax,0x004f
	jne svga_err
	mov al, [es:di]
	call print_char
	ret
	
svga_supported:
	mov ax,0x4f02
	mov bx,0x0112
	cmp ax,0x004f
	je svga_success
	jmp svga_err
	popa
	ret

svga_success:
	pop di
	pop es
	popa
	ret
	
svga_err:
	mov bx, SVGA_ERR_MSG
	call print_string
	ret
	
SVGA_ERR_MSG: db "Error starting SVGA.",0
SVGA_SUCCESS_MSG: db "SVGA init!",0