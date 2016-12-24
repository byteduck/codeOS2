#include <common.h>
#include <paging.h>
#include <heap.h>
#include <ata.h>
#include <stdio.h>
#include <ext2.h>

extern uint8_t ata_buf[512], ata_buf2[512];
ext2_partition current_ext2_partition = {};

bool isPartitionExt2(int disk, int sect){
	readSector(disk, sect+2, ata_buf); //Supercluster begins at partition sector + 2
	return ((ext2_superblock *)ata_buf)->signature != EXT2_SIGNATURE;
}

void getExt2Superblock(int disk, int sect, ext2_superblock *sp){
	readSector(disk, sect+2, (char *)sp);
	if(sp->version_major < 1){ //If major version is less than 1, then use defaults for stuff
		sp->first_inode = 11;
		sp->inode_size = 128;
	}
}

void setCurrentExt2Partition(int sect, uint8_t disk, ext2_superblock *sb){
	current_ext2_partition.sector = sect;
	current_ext2_partition.disk = disk;
	current_ext2_partition.block_size = 1024 << (sb->block_size);
	current_ext2_partition.block_group_descriptor_table = sb->superblock_block+1;
	current_ext2_partition.blocks_per_inode_table = (sb->inode_size*sb->inodes_per_group)/ext2_getBlockSize();
	current_ext2_partition.sectors_per_inode_table = (sb->inode_size*sb->inodes_per_group)/512;
	current_ext2_partition.sectors_per_block = ext2_getBlockSize()/512;
	current_ext2_partition.num_block_groups = sb->total_blocks/sb->blocks_per_group + (sb->total_blocks % sb->blocks_per_group != 0);
	current_ext2_partition.inodes_per_block = ext2_getBlockSize()/sb->inode_size;
	current_ext2_partition.superblock = sb;
}

uint32_t ext2_getBlockGroupOfInode(uint32_t node){
	return (node - 1) / ext2_getCurrentSuperblock()->inodes_per_group;
}

uint32_t ext2_getIndexOfInode(uint32_t node){
	return (node - 1) % ext2_getCurrentSuperblock()->inodes_per_group;
}

//inode is the actual number of the inode, not the index or anything.
uint32_t ext2_getBlockOfInode(uint32_t node){
	return (ext2_getIndexOfInode(node) * ext2_getCurrentSuperblock()->inode_size) / ext2_getBlockSize();
}

void ext2_readInode(uint32_t inode, ext2_inode *buf){
	uint32_t bg = ext2_getBlockGroupOfInode(inode);
	uint8_t *read = ext2_allocBlock();
	ext2_block_group_descriptor *d = (ext2_block_group_descriptor *)ext2_readBlock(2, read);
	for(int i = 0; i < bg; i++) d++; //note to self - d++ adds to the pointer by sizeof(ext2_block_group_descriptor)

	ext2_readBlock(d->inode_table+ext2_getBlockOfInode(inode), (uint8_t *)buf);
	ext2_inode *in = (ext2_inode *)buf;
	uint32_t index = ext2_getIndexOfInode(inode) % current_ext2_partition.inodes_per_block;
	for(int i = 0; i < index; i++) in++; //same here as above

	memcpy(buf, in, sizeof(ext2_inode));
	ext2_freeBlock(read);
}

void ext2_listDirectory(uint32_t inode_){
	ext2_inode *inode = kmalloc(sizeof(inode));
	ext2_readInode(inode_,inode);
	if((inode->type & 0xF000) != EXT2_DIRECTORY){
		printf("given inode is not a directory!\n");
	}else{
		uint8_t *buf = kmalloc(ext2_getBlockSize());
		for(int i = 0; i < 12; i++){
			uint32_t block = inode->block_pointers[i];
			if(block == 0 || block > ext2_getCurrentSuperblock()->total_blocks) break;
			ext2_readBlock(block, buf);
			ext2_listDirectoryEntries((ext2_directory *)buf);
		}
	}
}

void ext2_listDirectoryEntries(ext2_directory *dir){
	while(dir->inode != 0){
		char *name = kmalloc(dir->name_length + 1);
		name[dir->name_length] = '\0';
		memcpy(name, &dir->type+1, dir->name_length);
		if(name[0] != '.') printf("%s\n",name);
		kfree(name, dir->name_length + 1);
		dir = (ext2_directory *)((uint32_t)dir + dir->size);
	}
}

uint32_t ext2_findFileInDirectory(char *find_name, uint32_t dir_inode){
	ext2_inode *inode = kmalloc(sizeof(inode));
	ext2_readInode(dir_inode,inode);
	uint8_t *buf = kmalloc(ext2_getBlockSize());
	for(int i = 0; i < 12; i++){
		uint32_t block = inode->block_pointers[i];
		if(block == 0 || block > ext2_getCurrentSuperblock()->total_blocks) break;
		ext2_readBlock(block, buf);
		ext2_directory *dir = (ext2_directory *)buf;
		while(dir->inode != 0){
			char *name = kmalloc(dir->name_length + 1);
			name[dir->name_length] = '\0';
			memcpy(name, &dir->type+1, dir->name_length);
			if(strcmp(name, find_name)){
				kfree(name, dir->name_length + 1);
				return dir->inode;
			}
			kfree(name, dir->name_length + 1);
			dir = (ext2_directory *)((uint32_t)dir + dir->size);
		}
	}
	return 0;
}

ext2_superblock *ext2_getCurrentSuperblock(){
	return current_ext2_partition.superblock;
}

uint32_t ext2_getBlockSize(){
	return current_ext2_partition.block_size;
}

uint8_t *ext2_readBlock(uint32_t block, uint8_t *buf){
	readSectors(current_ext2_partition.disk, ext2_blockToSector(block), current_ext2_partition.sectors_per_block, buf);
	return buf;
}

uint8_t *ext2_allocBlock(){
	return kmalloc(ext2_getBlockSize());
}

void ext2_freeBlock(uint8_t *block){
	kfree(block, ext2_getBlockSize());
}

uint32_t ext2_blockToSector(uint32_t block){
	return current_ext2_partition.sector+(ext2_getBlockSize()/512)*block;
}
