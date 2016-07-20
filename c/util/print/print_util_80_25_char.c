void putch_color(char c, char color){
	unsigned char* vidmem = (char*)0xb8000;
	if(c == '\r'){
		xpos = 0;
	}else if(c == '\n'){
		xpos = 0;
		ypos++;
	}else{
		int pos = (xpos+(ypos*80))*2;
		vidmem[pos] = c;
		vidmem[pos+1] = color;
		xpos++;
		if(xpos >= 80){
			ypos++;
			xpos = 0;
			if(ypos >= 25){
				ypos = 0;
			}
		}
	}
}

void clearScreen(){
	unsigned char* vidmem = (char*)0xb8000;
	for(int y=0; y<25; y++){
		for(int x=0; x<80; x++){
			vidmem[(x+(y*80))*2] = ' ';
		}
	}
	xpos = 0;
	ypos = 0;
}

void setAllColor(char color){
	unsigned char* vidmem = (char*)0xb8000;
	for(int y=0; y<25; y++){
		for(int x=0; x<80; x++){
			vidmem[(x+(y*80))*2+1] = color;
		}
	}
	setColor(color);
}

void center_print(char* c, char color){
	center_print_base(c, color, 80);
}