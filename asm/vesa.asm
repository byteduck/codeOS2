init_vesa:
	pusha
	push es
	push di
	mov ax,0x1000
	mov es,ax
	mov di,0
	call vesa_query
	mov ax,0x4f01
	mov cx,0x0112 ;640*480*24 mode
	int 0x10
	cmp ax,0x004f
	je vesa_supported
	jmp vesa_err
	popa
	ret

vesa_query:
	mov ax,0x4f00
	int 0x10
	cmp ax,0x004f
	jne vesa_err
	mov ax,[es:di]
	cmp ax,'VE'   ;First 4 bytes of VGA info block should be VESA if it is valid, so we're checking the first two.
	jne vesa_err
	ret
	
vesa_supported:
	mov ax,0x4f02
	mov bx,0x0112
	int 0x10
	cmp ax,0x004f
	je vesa_success
	jmp vesa_err
	popa
	ret

vesa_success:
	pop di
	pop es
	popa
	ret
	
vesa_err:
	mov bx, VESA_ERR_MSG
	call print_string
	ret
	
VESA_ERR_MSG: db "Error starting VESA.",0x20,0