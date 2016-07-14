void putch(char c);
void print(char *c);

int xpos = 0;
int ypos = 0;
char ccolor = 0x07;

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

void putch(char c){
	putch_color(c, ccolor);
}

void print_color(char* c, char color){
	int i = 0;
	while(c[i] != 0){
		putch_color(c[i], color);
		i++;
	}
}

void print(char* c){
	print_color(c, ccolor);
}

void println_color(char* c, char color){
	print_color(c, color);
	print_color("\n", color);
}

void println(char* c){
	println_color(c,ccolor);
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

void setColor(char color){
	ccolor = color;
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
	if(xpos > 0){
		print_color("\n",color);
	}
	int i = 0;
	while(c[i]){
		i++;
	}
	if(i > 80){
		print_color(c,color);
	}else{
		if(i % 2 == 0){
			int h = (80-i)/2;
			int j = 0;
			while(j < h){
				putch_color(' ', color);
				j++;
			}
			print_color(c,color);
			j = 0;
			while(j < h){
				putch_color(' ', color);
				j++;
			}
		}else{
			int h = (80-i)/2;
			int j = 0;
			while(j < h){
				putch_color(' ', color);
				j++;
			}
			print_color(c,color);
			j = 0;
			h--;
			while(j < h+2){
				putch_color(' ', color);
				j++;
			}
		}
	}
}