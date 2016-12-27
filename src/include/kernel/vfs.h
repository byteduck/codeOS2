typedef struct filesystem_t{
	bool (*probe)(uint8_t disk);
	bool (*exists)(char *fn, uint8_t disk, void *);
}
