#include <common.h>
#include <keyboard.h>
#include <stdio.h>
#include <shell.h>
#include <ext2.h>
#include <heap.h>

char cmdbuf[256];
char argbuf[256];
char dirbuf[512];
bool exitShell = false;
uint32_t current_inode = 2;
ext2_inode *inode_buf;
extern bool shell_mode;
extern uint8_t kbdbuf[256];
ext2_partition *ext2;

void shell(ext2_partition *ext2p){
	ext2 = ext2p;
	inode_buf = kmalloc(sizeof(ext2_inode));
	while(!exitShell){
		print("codeOS2:/$ ");
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
	}else if(strcmp(cmd,"ls")){
		if(strcmp(args,""))
			ext2_listDirectory(current_inode, ext2);
		else{
			uint32_t inodeID = ext2_findFile(args, current_inode, inode_buf, ext2);
			if(!inodeID) printf("That directory does not exist.\n"); else{
				ext2_inode *inode = kmalloc(sizeof(ext2_inode));
				ext2_readInode(inodeID, inode, ext2);
				if((inode->type & 0xF000) != EXT2_DIRECTORY) printf("%s is not a directory.\n",args); else ext2_listDirectory(inodeID, ext2);
			}
		}
	}else if(strcmp(cmd,"cd")){
		uint32_t inodeID = ext2_findFile(args, current_inode, inode_buf, ext2);
		if(!inodeID) printf("That directory does not exist.\n"); else{
			ext2_inode *inode = kmalloc(sizeof(ext2_inode));
			ext2_readInode(inodeID, inode, ext2);
			if((inode->type & 0xF000) != EXT2_DIRECTORY) printf("%s is not a directory.\n",args); else current_inode = inodeID;
		}
	}else if(strcmp(cmd,"pwd")){
		/*if(strcmp(args,"-c")){
			//printHex(currentfat32part.current_dir_clust);
			println("");
		}else{
			printCurrentDir();
		}*/
	}else if(strcmp(cmd,"about")){
		println("CodeOS2 v0.0");
	}else if(strcmp(cmd, "partinfo")){
		printf("Disk: %d\nBlock size: %d (%d sectors)\nBlocks per group: %d (%d block groups)\nInodes per group: %d\nSuperblock sector: %d\nInode table size: %d blocks\nName: %s\n",
		ext2->disk, ext2_getBlockSize(ext2), ext2->sectors_per_block, ext2_getSuperblock(ext2)->blocks_per_group, ext2->num_block_groups, ext2_getSuperblock(ext2)->inodes_per_group, ext2->sector+2, ext2->blocks_per_inode_table, ext2_getSuperblock(ext2)->volume_name);
	}else if(strcmp(cmd, "inode")){
		if(strcmp(args, ""))
			printf("Usage: \ninode <inode #> - prints information about an inode.\n");
		else{
			ext2_inode *inode = kmalloc(sizeof(ext2_inode));
			ext2_readInode(strToInt(args),inode,ext2);
			printf("inode %d:\n",strToInt(args));
			printf("inode type: 0x%x\ninode size: 0x%x bytes\n", inode->type, inode->size_lower);
			printf("inode flags: %b\n", inode->flags);
		}
	}else if(strcmp(cmd,"cat")){
		if(ext2_findFile(args, current_inode, inode_buf,ext2)){
			uint8_t *fbuf = kmalloc(inode_buf->sectors_in_use*512);
			if(ext2_readFile(inode_buf, fbuf, ext2)){
				for(uint32_t i = 0; i < inode_buf->size_lower; i++)
					putch(fbuf[i]);
			}else{
				printf("Error reading file.");
			}
			kfree(fbuf, inode_buf->sectors_in_use*512);
		}else{
			printf("File %s does not exist.\n", args);
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
	}else{
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
