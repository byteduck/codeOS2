#ifndef EXT2_H
#define EXT2_H

typedef struct ext2_superblock{
	uint32_t total_inodes;
	uint32_t total_blocks;
	uint32_t super_blocks;
	uint32_t unallocated_blocks;
	uint32_t unallocated_inodes;
	uint32_t superblock_block;
	uint32_t block_size; //do 1024 << block_size to get the block size
	uint32_t fragment_size; //Do 1024 << fragment_size to get the fragment size
	uint32_t blocks_per_group;
	uint32_t fragments_per_group;
	uint32_t inodes_per_group;
	uint32_t last_mount;
	uint32_t last_write;
	uint16_t times_mounted; //Since last fsck
	uint16_t mounts_allowed; //fsck must be done after this amount surpassed
	uint16_t signature;
	uint16_t state;
	uint16_t error_action;
	uint16_t version_minor;
	uint32_t last_check; //POSIX time of last fsck
	uint32_t check_interval; //POSIX time between fscks
	uint32_t os_id;
	uint32_t version_major;
	uint16_t reserved_user;
	uint16_t reserved_group; 
	//Start extended fields
	uint32_t first_non_reserved_inode;
	uint16_t inode_size;
	uint16_t superblock_group;
	uint32_t optional_features;
	uint32_t required_features;
	uint32_t ro_features;
	uint8_t filesystem_id[16];
	uint8_t volume_name[16];
	uint8_t last_mount_path[64];
	uint32_t compression;
	uint8_t file_prealloc_blocks;
	uint8_t directory_prealloc_blocks;
	uint16_t UNUSED_AARON_IS_THE_BEST;
	uint8_t journal_id[16];
	uint32_t journal_inode;
	uint32_t journal_device;
	uint32_t orphan_inode_head;
	uint8_t extra[273];
} ext2_superblock;

typedef struct ext2_partition{
	uint32_t sector;
	uint8_t disk;
	ext2_superblock *superblock;
	
} ext2_partition;

bool isPartitionExt2(int disk, int sect);
void getExt2Superblock(int disk, int sect, ext2_superblock *sp);
void setCurrentExt2Partition(int sect, uint8_t disk, ext2_superblock *sb);
uint32_t getBlockSize(ext2_superblock *sb);

#endif
