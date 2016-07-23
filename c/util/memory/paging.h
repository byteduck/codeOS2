struct Page{
	uint32_t present:1;
	uint32_t readwrite:1;
	uint32_t usermode:1;
	uint32_t accessed:1;
	uint32_t written:1;
	uint32_t unused:7;
	uint32_t frame:20;
};

struct PageTable{
	struct Page pages[1024];
};

struct PageDirectory{
	struct PageTable *tables[1024];
	uint32_t tablesPhysical[1024];
	uint32_t physicalAddress;
};

void paging_init();
void selectPageDirectory(struct PageDirectory *directory);
struct Page *getPage(uint32_t addr, bool make, struct PageDirectory *dir);
void pageFaultHandler(struct registers registers);
void setFrame(uint32_t frameAddress);
void clearFrame(uint32_t frameAddress);
uint32_t testFrame(uint32_t frameAddress);
uint32_t firstFrame();
void allocateFrame(struct Page *page, bool kernel, bool writeable);
void freeFrame(struct Page *page);

#include "paging.c"