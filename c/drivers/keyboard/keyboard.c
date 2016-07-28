bool shift = 0;

void keyboard_handler(struct registers *r){
    uint8_t scancode;
	
    scancode = inb(0x60);
	
    if (scancode & 0x80){
		if(scancode == 0xAA)
			shift = false;
    }else{
		if(scancode == 0x2a){shift = 1;return;}
		if(scancode == 0x1c){putch('\n');return;}
		if(scancode == 0x0e){
			backspace();
			return;
		}
		if(isACharacter(kbdus[scancode])){
			if(shift){
				putch(kbdus[scancode+90]);
			}else{
				putch(kbdus[scancode]);
			}
		}
    }
}