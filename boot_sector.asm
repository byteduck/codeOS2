[org 0x7c00] ;Tell BIOS where to load this in memory to give the stack more space
mov bx, HELLO_WORLD
call print_string
mov bx, GOODBYE_WORLD
call print_string
mov bx, OH_WAIT
call print_string
mov dx, 0x1234
call print_hex
jmp $

print_string: ;Prints String at bx
	pusha
	mov ah, 0x0e
print_loop:
	mov al, [bx]
	cmp al, 0
	je print_string_done
	int 0x10
	add bx, 1
	jmp print_loop
print_string_done:
	popa
	ret
	
print_char:
	pusha
	mov ah, 0x0e
	int 0x10
	popa
	ret
	
print_hex:
	pusha
	mov CL, 4
	times 4 call hex_loop
	popa
	ret
hex_loop:
	mov dx,cx
	and cx,0xf
	add cx, 0x30
	shr dx,4
	mov ch,al
	call print_char
	ret

HELLO_WORLD:
	db 'Hello world! ',0
	
GOODBYE_WORLD:
	db 'Goodbye world! ',0
	
OH_WAIT:
	db 'Oh wait, that was more morbid than I expected. ',0
	
PING_CMD:
	db 'ping'
	
PING_RESULT:
	db 'pong!'

times 510 -( $ - $$ ) db 0
dw 0xaa55