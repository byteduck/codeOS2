#include <common.h>
#include <keyboard.h>
#include <stdio.h>
#include <shell.h>
#include <ext2.h>

char cmdbuf[256];
char argbuf[256];
bool exitShell = false;
extern bool shell_mode;
extern uint8_t kbdbuf[256];
extern ext2_partition current_ext2_partition;

void shell(){
	while(!exitShell){
		print("root@codeOS2:/$ ");
		shell_mode = true;
		getInput();
		shell_mode = false;
		setColor(0x07);
		substr(indexOf(' ',kbdbuf), kbdbuf, cmdbuf);
		if(indexOf(' ', kbdbuf)+1 <= strlen(kbdbuf)){
			substrr(indexOf(' ', kbdbuf)+1, strlen(kbdbuf), kbdbuf, argbuf);
		}else{
			argbuf[0] = 0;
		}
		command_eval(cmdbuf, argbuf);
		setColor(0x0f);
	}
}

static void command_eval(char *cmd, char *args){
	if(strcmp(cmd,"help")){
		println("ls: List the files in the current directory. Use -h for help.");
		println("cd: Change the current directory.");
		println("pwd: Print the working directory.");
		println("about: Shows some information about the system.");
		println("help: Shows this message.");
		println("cat: Prints a file's contents.");
		println("about: Prints some information.");
		println("partinfo: Prints information about the current partition.");
		println("pagefault: Triggers a page fault, in case you wanted to.");
		println("exit: Pretty self explanatory.");
	}/*else if(strcmp(cmd,"ls")){
		if(strcmp(args,"-h")){
			println("Usage: ls [contains]");
		}else if(!strcmp(args,"")){
			listCurrentDir(args);
		}else{
			listCurrentDir("");
		}
	}else if(strcmp(cmd,"cd")){
		switch(changeDir(args)){
			case 1:
			print("\"");
			print(args);
			println("\" is not a directory.");
			break;
			case 2:
			print("\"");
			print(args);
			println("\" does not exist.");
			break;
		}
	}else if(strcmp(cmd,"pwd")){
		if(strcmp(args,"-c")){
			//printHex(currentfat32part.current_dir_clust);
			println("");
		}else{
			printCurrentDir();
		}
	}else if(strcmp(cmd,"about")){
		println("CodeOS2 v0.0");
	}*/else if(strcmp(cmd, "partinfo")){
		print("Disk: ");
		printHex(current_ext2_partition.disk);
		print("\nBlock size: ");
		printHexw(getBlockSize(current_ext2_partition.superblock));
		print("\nBlocks per group: ");
		printHexl(current_ext2_partition.superblock->blocks_per_group);
	}/*else if(strcmp(cmd,"cat")){
		fat32file f = getFile(args);
		if(exists(f)){
			printFileContents(f);
		}else{
			println("File doesn't exist!");
		}
	}else if(strcmp(cmd,"pagefault")){
		if(strcmp(args,"-r")){
			char i = ((char*)0xDEADC0DE)[0];
		}else if(strcmp(args,"-w")){
			((char*)0xDEADC0DE)[0]='F';
		}else{
			println("Usage: pagefault [-r,-w]");
			println("-r: Triggers a page fault by reading.");
			println("-w: Triggers a page fault by writing.");
		}
	}else if(strcmp(cmd,"exit")){
		exitShell = true;
	}*/else{
		/*fat32file f = getFile(cmd);
		if(exists(f) && !isDirectory(f))
			executeFile(f);
		else{*/
			print("\"");
			print(cmd);
			println("\" is not a recognized command, file, or program.");
		//}
	}
}
