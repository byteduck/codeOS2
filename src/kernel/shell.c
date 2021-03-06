#include <common.h>
#include <keyboard.h>
#include <stdio.h>
#include <shell.h>
#include <ext2.h>
#include <heap.h>
#include <tasking.h>

char cmdbuf[256];
char argbuf[256];
char dirbuf[512];
char dirbuf2[512];
bool exitShell = false;
file_t currentDir = {}, fileBuf = {};
extern bool shell_mode;
extern uint8_t kbdbuf[256];
filesystem_t *fs;
extern bool tasking_enabled;

void initShell(filesystem_t *fsp){
	fs = fsp;
	fs->getFile("/",&currentDir,fs);
	dirbuf[0] = '/';
	dirbuf[1] = '\0';
}

void dummy(){
	while(1);
}

void shell(){
	while(!exitShell){
		printf("codeOS2:%s$ ", dirbuf);
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
	__kill__();
}

uint8_t prog[0x1000];

void progx(){
	exec(prog);
}

bool findAndExecute(char *cmd, bool wait){
	file_t *file = kmalloc(sizeof(file_t));
	if(fs->getFile(cmd, file, fs) && !file->isDirectory){
		if(file->sectors*512 > 0x1000)
			printf("Executable too large.\n");
		else{
			fs->read(file, prog, fs);
			process_t *proc = createProcess(cmd, (uint32_t)progx);
			uint32_t pid = addProcess(proc);
			while(wait && proc->state == PROCESS_ALIVE);
		}
		kfree(file, sizeof(file_t));
		return true;
	}
	return false;
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
		//println("partinfo: Prints information about the current partition.");
		println("pagefault: Triggers a page fault, in case you wanted to.");
		println("tasks: Prints all running tasks.");
		println("bg: Run a program in the background.");
		println("kill: Kill a program.");
		println("dummy: Create a dummy process.");
		println("exit: Pretty self explanatory.");
	}else if(strcmp(cmd,"ls")){
		if(strcmp(args,"")){
			fs->listDir(&currentDir, fs);
		}else{
			/*uint32_t inodeID = ext2_findFile(args, current_inode, inode_buf, ext2);
			if(!inodeID) printf("That directory does not exist.\n"); else{
				ext2_inode *inode = kmalloc(sizeof(ext2_inode));
				ext2_readInode(inodeID, inode, ext2);
				if((inode->type & 0xF000) != EXT2_DIRECTORY) printf("%s is not a directory.\n",args); else ext2_listDirectory(inodeID, ext2);
			}*/
		}
	}else if(strcmp(cmd,"cd")){
		strcpy(dirbuf, dirbuf2);
		strcat(dirbuf2,args);
		strcat(dirbuf2,"/");
		if(!fs->getFile(dirbuf2, &fileBuf, fs)) printf("That directory does not exist.\n"); else{
			if(!fileBuf.isDirectory) printf("%s is not a directory.\n",args); else{
				currentDir = fileBuf;
				strcpy(dirbuf2, dirbuf);
			}
		}
	}else if(strcmp(cmd,"pwd")){
		printf("%s\n",dirbuf);
	}else if(strcmp(cmd,"about")){
		println("CodeOS2 v0.0");
	}/*else if(strcmp(cmd, "partinfo")){
		printf("Disk: %d\nBlock size: %d (%d sectors)\nBlocks per group: %d (%d block groups)\nInodes per group: %d\nSuperblock sector: %d\nInode table size: %d blocks\nName: %s\n",
		ext2->disk, ext2_getBlockSize(ext2), ext2->sectors_per_block, ext2_getSuperblock(ext2)->blocks_per_group, ext2->num_block_groups, ext2_getSuperblock(ext2)->inodes_per_group, ext2->sector+2, ext2->blocks_per_inode_table, ext2_getSuperblock(ext2)->volume_name);
	}/*else if(strcmp(cmd, "inode")){
		if(strcmp(args, ""))
			printf("Usage: \ninode <inode #> - prints information about an inode.\n");
		else{
			ext2_inode *inode = kmalloc(sizeof(ext2_inode));
			ext2_readInode(strToInt(args),inode,ext2);
			printf("inode %d:\n",strToInt(args));
			printf("inode type: 0x%x\ninode size: 0x%x bytes\n", inode->type, inode->size_lower);
			printf("inode flags: %b\n", inode->flags);
		}
	}*/else if(strcmp(cmd,"cat")){
		file_t file = {};
		strcpy(dirbuf, dirbuf2);
		strcat(dirbuf2,args);
		strcat(dirbuf2,"/");
		if(fs->getFile(dirbuf2, &file, fs)){
			uint8_t *buf = kmalloc(file.sectors*512);
			fs->read(&file, buf, fs);
			for(int i = 0; i < file.size; i++)
				putch(buf[i]);
			kfree(buf, file.sectors*512);
		}else{
			printf("Cannot find %s.\n",args);
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
	}else if(strcmp(cmd,"tasks")){
		printTasks();
	}else if(strcmp(cmd,"bg")){
		if(strcmp(args,"") || !findAndExecute(args, false)) printf("Cannot find \"%s\".\n", args);
	}else if(strcmp(cmd,"kill")){
		uint32_t pid = strToInt(args);
		process_t *proc = getProcess(pid);
		if(proc != NULL && proc->pid != 1){
			kill(proc);
			printf("Sent SIGTERM (%d) to %s (PID %d).\n", SIGTERM, proc->name, proc->pid);
		}else if(proc->pid == 1)
			printf("Cannot kill kernel!\n");
		else
			printf("No process with PID %d.\n", pid);
	}else if(strcmp(cmd, "dummy")){
		addProcess(createProcess("dummy", (uint32_t)dummy));
	}else{
		if(!findAndExecute(cmd, true)) printf("\"%s\" is not a recognized command, file, or program.\n", cmd);
	}
}
