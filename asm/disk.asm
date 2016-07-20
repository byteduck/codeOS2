disk_load: ;Loads DH sectors into ES:BX from drive DL
              ;Only works in 16-bit mode
push dx
mov ah,0x02
mov al,dh ;Read DH sectors
mov ch,0x00 ;Cylinder 0
mov dh,0x00 ;Head 0
cmp cl,0x00
je disk_load_go
mov cl,0x02 ;Start from second sector if cl is not set
disk_load_go:
int 0x13
jc disk_error ;Something went wrong
pop dx
cmp dh,al ;Compare sectors expected vs sectors gotten
jne disk_error
ret

disk_error:
mov bx , DISK_ERROR_MSG
call print_string
jmp $