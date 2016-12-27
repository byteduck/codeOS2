#include <common.h>
#include <paging.h>
#include <heap.h>
#include <ata.h>
#include <stdio.h>
#include <ext2.h>

extern uint8_t ata_buf[512], ata_buf2[512];

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

void initExt2Partition(int sect, uint8_t disk, ext2_superblock *sb, ext2_partition *part){
	part->sector = sect;
	part->disk = disk;
	part->block_size = 1024 << (sb->block_size);
	part->block_group_descriptor_table = sb->superblock_block+1;
	part->blocks_per_inode_table = (sb->inode_size*sb->inodes_per_group)/ext2_getBlockSize(part);
	part->sectors_per_inode_table = (sb->inode_size*sb->inodes_per_group)/512;
	part->sectors_per_block = ext2_getBlockSize(part)/512;
	part->num_block_groups = sb->total_blocks/sb->blocks_per_group + (sb->total_blocks % sb->blocks_per_group != 0);
	part->inodes_per_block = ext2_getBlockSize(part)/sb->inode_size;
	part->superblock = sb;
}

uint32_t ext2_getBlockGroupOfInode(uint32_t node, ext2_partition *part){
	return (node - 1) / ext2_getSuperblock(part)->inodes_per_group;
}

uint32_t ext2_getIndexOfInode(uint32_t node, ext2_partition *part){
	return (node - 1) % ext2_getSuperblock(part)->inodes_per_group;
}

//inode is the actual number of the inode, not the index or anything.
uint32_t ext2_getBlockOfInode(uint32_t node, ext2_partition *part){
	return (ext2_getIndexOfInode(node, part) * ext2_getSuperblock(part)->inode_size) / ext2_getBlockSize(part);
}

void ext2_readInode(uint32_t inode, ext2_inode *buf, ext2_partition *part){
	uint32_t bg = ext2_getBlockGroupOfInode(inode, part);
	uint8_t *read = ext2_allocBlock(part);
	ext2_block_group_descriptor *d = (ext2_block_group_descriptor *)ext2_readBlock(2, read, part);
	for(int i = 0; i < bg; i++) d++; //note to self - d++ adds to the pointer by sizeof(ext2_block_group_descriptor)

	ext2_readBlock(d->inode_table+ext2_getBlockOfInode(inode, part), (uint8_t *)buf, part);
	ext2_inode *in = (ext2_inode *)buf;
	uint32_t index = ext2_getIndexOfInode(inode, part) % part->inodes_per_block;
	for(int i = 0; i < index; i++) in++; //same here as above

	memcpy(buf, in, sizeof(ext2_inode));
	ext2_freeBlock(read, part);
}

void ext2_listDirectory(uint32_t inode_, ext2_partition *part){
	ext2_inode *inode = kmalloc(sizeof(inode));
	ext2_readInode(inode_,inode,part);
	if((inode->type & 0xF000) != EXT2_DIRECTORY){
		printf("given inode is not a directory!\n");
	}else{
		uint8_t *buf = ext2_allocBlock(part);
		for(int i = 0; i < 12; i++){
			uint32_t block = inode->block_pointers[i];
			if(block == 0 || block > ext2_getSuperblock(part)->total_blocks) break;
			ext2_readBlock(block, buf, part);
			ext2_listDirectoryEntries((ext2_directory *)buf);
		}
		ext2_freeBlock(buf, part);
	}
	kfree(inode, sizeof(inode));
}

void ext2_listDirectoryEntries(ext2_directory *dir){
	while(dir->inode != 0 && dir->size != 0){
		char *name = kmalloc(dir->name_length + 1);
		name[dir->name_length] = '\0';
		memcpy(name, &dir->type+1, dir->name_length);
		if(name[0] != '.' && name[0] != '\0') printf("%s\n", name, dir->inode);
		kfree(name, dir->name_length + 1);
		dir = (ext2_directory *)((uint32_t)dir + dir->size);
	}
}

//dir_inode will be set to 2 if find_name starts with '/'
uint32_t ext2_findFile(char *find_name, uint32_t dir_inode, ext2_inode *inode, ext2_partition *part){
	if(find_name[0] == '/'){
		find_name++;
		dir_inode = 2;
	}
	uint32_t namelen = strlen(find_name);
	if(namelen == 0) return dir_inode;
	uint8_t *buf = ext2_allocBlock(part);
	char *cfind_name = kmalloc(namelen);
	while(*find_name != 0){
		uint32_t strindex = indexOf('/',find_name);
		substrr(0,strindex,find_name,cfind_name);
		find_name+=strindex+(strindex == strlen(find_name) ? 0 : 1);
		ext2_readInode(dir_inode,inode,part);
		bool found = 0;
		for(int i = 0; i < 12 && !found; i++){
			uint32_t block = inode->block_pointers[i];
			if(block == 0 || block > ext2_getSuperblock(part)->total_blocks) break;
			ext2_readBlock(block, buf, part);
			ext2_directory *dir = (ext2_directory *)buf;
			while(dir->inode != 0 && dir->size != 0 && !found){
				char *name = kmalloc(dir->name_length + 1);
				name[dir->name_length] = '\0';
				memcpy(name, &dir->type+1, dir->name_length);
				if(strcmp(name, cfind_name)){
					dir_inode = dir->inode;
					ext2_readInode(dir_inode,inode,part);
					found = 1;
				}
				kfree(name, dir->name_length + 1);
				dir = (ext2_directory *)((uint32_t)dir + dir->size);
			}
		}
		if(!found){
			dir_inode = 0;
			find_name+= strlen(find_name);
		}
	}
	kfree(cfind_name, namelen);
	ext2_freeBlock(buf,part);
	return dir_inode;
}

//buf must be a multiple of the blocksize
bool ext2_readFile(ext2_inode *inode, uint8_t *buf, ext2_partition *part){
	for(int i = 0; i < 12; i++){
		uint32_t block = inode->block_pointers[i];
		if(block == 0 || block > ext2_getSuperblock(part)->total_blocks){break;}
		ext2_readBlock(block, buf, part);
	}
	if(inode->s_pointer)
		ext2_read_slink(inode->s_pointer, buf+12*ext2_getBlockSize(part), part);
	if(inode->d_pointer)
		ext2_read_dlink(inode->d_pointer, buf+(ext2_getBlockSize(part)/sizeof(uint32_t))*ext2_getBlockSize(part)+12*ext2_getBlockSize(part), part);
	if(inode->t_pointer)
		printf("WARNING! File uses t_pointer. Will not work.\n");
	return 1;
}

void ext2_read_slink(uint32_t block, uint8_t *buf, ext2_partition *part){
	uint8_t *bbuf = ext2_allocBlock(part);
	ext2_readBlock(block, bbuf, part);
	uint32_t *blocks = (uint32_t *)bbuf;
	uint32_t numblocks = ext2_getBlockSize(part)/sizeof(uint32_t);
	for(int i = 0; i < numblocks; i++){
		if(blocks[i] == 0) break;
		ext2_readBlock(blocks[i], buf+i*ext2_getBlockSize(part), part);
	}
	ext2_freeBlock(bbuf,part);
}

void ext2_read_dlink(uint32_t block, uint8_t *buf, ext2_partition *part){
	uint8_t *bbuf = ext2_allocBlock(part);
	ext2_readBlock(block, bbuf, part);
	uint32_t *blocks = (uint32_t *)bbuf;
	uint32_t numblocks = ext2_getBlockSize(part)/sizeof(uint32_t);
	uint32_t singsize = numblocks*ext2_getBlockSize(part);
	for(int i = 0; i < numblocks; i++){
		if(blocks[i] == 0) break;
		ext2_readBlock(blocks[i], buf+i*singsize, part);
	}
	ext2_freeBlock(bbuf, part);
}

ext2_superblock *ext2_getSuperblock(ext2_partition *part){
	return part->superblock;
}

uint32_t ext2_getBlockSize(ext2_partition *part){
	return part->block_size;
}

uint8_t *ext2_readBlock(uint32_t block, uint8_t *buf, ext2_partition *part){
	readSectors(part->disk, ext2_blockToSector(block, part), part->sectors_per_block, buf);
	return buf;
}

uint8_t *ext2_allocBlock(ext2_partition *part){
	return kmalloc(ext2_getBlockSize(part));
}

void ext2_freeBlock(uint8_t *block, ext2_partition *part){
	kfree(block, ext2_getBlockSize(part));
}

uint32_t ext2_blockToSector(uint32_t block, ext2_partition *part){
	return part->sector+(ext2_getBlockSize(part)/512)*block;
}
