#include "keyboard.h"
#include "kbdus.c"

bool shift = 0;
char kbdbuf[256];
volatile bool input_mode = false;
volatile bool input_done = false;
uint8_t input_i;
volatile bool shell_mode = false;

void keyboard_handler(struct registers *r){
    uint8_t scancode;
	
    scancode = inb(0x60);
	
    if (scancode & 0x80){
		if(scancode == 0xAA)
			shift = false;
    }else{
		if(scancode == 0x2a){shift = 1;return;}
		if(scancode == 0x1c){
			putch('\n');
			if(input_mode)
				input_done = true;
				return;
			}
		if(scancode == 0x0e){
			if(shell_mode){
				if(input_i > 0){
					backspace();
				}
			}else{
				backspace();
			}
			
			if(input_mode && input_i > 0)
				input_i--;
			
			return;
		}
		if(isACharacter(kbdus[scancode])){
			if(shift){
				putch(kbdus[scancode+90]);
				if(input_mode){
					kbdbuf[input_i] = kbdus[scancode+90];
					input_i++;
				}
			}else{
				putch(kbdus[scancode]);
				if(input_mode){
					kbdbuf[input_i] = kbdus[scancode];
					input_i++;
				}
			}
		}
    }
}

void getInput(){ //Puts characters into kbdbuf until enter is pressed or you reach 255 characters
	input_mode = true;
	input_done = false;
	input_i = 0;
	while(!input_done && input_i < 255){}
	kbdbuf[input_i] = '\0';
	input_mode = false;
}