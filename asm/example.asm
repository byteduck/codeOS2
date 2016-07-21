mov ax, 0      ;Set ax to zero, we'll use it as our counter.
loop_begin:    ;This isn't actually code, it's just defining a location where we can jump to. We don't use loop: because that's a reserved keyword.
cmp ax, 15     ;Compare ax to 15.
jg loop_done   ;Go to loop_done if ax is more than 15. Otherwise, continue.
DO SOMETHING HERE
add ax, 1      ;Increment ax by one. Alternatively, you can use inc ax.
jmp loop_begin ;Go back to the start of the loop.
loop_done:     ;Again, just a location. Not code.
               ;Now we're done with the loop

;This is the equivalent of:

;for(int i = 0; i <= 15; i++){
;	//Do something
;}