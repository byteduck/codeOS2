[global pushc]
pushc:
	push ebp           ;
	mov ebp, esp       ;Set the stack location to where we stored the passed variables in C
	push dword [ebp+8] ;
	pop eax            ;The thing we're going to push
	mov esp, ebp ;
	pop ebp      ;Restore the stack
	push eax     ;Push the thing we're gonna push
	ret
[global popc]
popc:
	pop eax
	push eax
	ret