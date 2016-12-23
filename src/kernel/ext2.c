#include <common.h>
#include <paging.h>
#include <heap.h>
#include <ata.h>
#include <stdio.h>
#include <ext2.h>

uint16_t ext2sig = 0x53ef;
extern uint8_t buf[512], buf2[512];
ext2_partition current_ext2_partition = {};

bool isPartitionExt2(int disk, int sect){
	readSector(disk,sect+2, buf); //Supercluster begins at partition sector + 2
	return ((ext2_superblock *)buf)->signature != ext2sig;
}

void getExt2Superblock(int disk, int sect, ext2_superblock *sp){
	readSector(disk, sect+2, (char *)sp);
	if(sp->version_major < 1){ //If major version is less than 1, then use defaults for stuff
		sp->first_non_reserved_inode = 11;
		sp->inode_size = 128;
	}
}

void setCurrentExt2Partition(int sect, uint8_t disk, ext2_superblock *sb){
	current_ext2_partition.sector = sect;
	current_ext2_partition.disk = disk;
	current_ext2_partition.superblock = sb;
}

uint32_t getBlockSize(ext2_superblock *sb){
	return 1024 << (sb->block_size);
}
