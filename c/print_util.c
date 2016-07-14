void putch(char c);
void print(char *c);

int xpos = 0;
int ypos = 0;

void putch(char c){
	unsigned char* vidmem = (char*)0xb8000;
	if(c == '\r'){
		xpos = 0;
	}else if(c == '\n'){
		xpos = 0;
		ypos++;
	}else{
		vidmem[(xpos+(ypos*80))*2] = c;
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

void print(char* c){
	int i = 0;
	while(c[i] != 0){
		putch(c[i]);
		i++;
	}
}

void println(char* c){
	print(c);
	print("\n");
}

void clear_screen(){
	unsigned char* vidmem = (char*)0xb8000;
	for(int y=0; y<25; y++){
		for(int x=0; x<80; x++){
			vidmem[(x+(y*80))*2] = ' ';
		}
	}
	xpos = 0;
	ypos = 0;
}