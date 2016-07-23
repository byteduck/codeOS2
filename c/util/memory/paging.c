/*
* Credit to James Molloy for the paging code
*/

//Page directory for kernel
struct PageDirectory *kernelDirectory=0;

// The current page directory;
struct PageDirectory *currentDirectory=0;

uint32_t *frames;
uint32_t frameCount;

extern uint32_t placement_address; //In heap.c

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

void setFrame(uint32_t frameAddress){
   uint32_t frame = frameAddress/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

void clearFrame(uint32_t frameAddress){
   uint32_t frame = frameAddress/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

uint32_t testFrame(uint32_t frameAddress){
   uint32_t frame = frameAddress/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

uint32_t firstFrame(){
   uint32_t i, j;
   for (i = 0; i < INDEX_FROM_BIT(frameCount); i++){
       if (frames[i] != 0xFFFFFFFF){
           for (j = 0; j < 32; j++){
               uint32_t toTest = 0x1 << j;
               if ( !(frames[i]&toTest) ){
                   return i*4*8+j;
               }
           }
       }
   }
}

void allocateFrame(struct Page *page, bool kernel, bool writeable){
	if (page->frame != 0){ //Already allocated
       return;
   }else{
       uint32_t idx = firstFrame();
       if (idx == (uint32_t)-1){
           //TODO no free frames
       }
       setFrame(idx*0x1000);
       page->present = 1;
       page->readwrite = (writeable)?1:0;
       page->usermode = (kernel)?0:1;
       page->frame = idx;
   }
}

void freeFrame(struct Page *page){
	uint32_t frame;
	if (!(frame=page->frame)){
		return; //There's no frame here...
	}else{
		clearFrame(frame);
		page->frame = 0x0;
	}
}

void paging_init(){
   uint32_t mem_end_page = 0x1000000; //We are assuming 16MB of RAM

   frameCount = mem_end_page / 0x1000;
   frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(frameCount));
   memset(frames, 0, INDEX_FROM_BIT(frameCount));

   kernelDirectory = (struct PageDirectory*)kmalloc_a(sizeof(struct PageDirectory));
   memset(kernelDirectory, 0, sizeof(struct PageDirectory));
   currentDirectory = kernelDirectory;
   
   int i = 0;
   while (i < placement_address){
       allocateFrame( getPage(i, 1, kernelDirectory), 0, 0);
       i += 0x1000;
   }

   selectPageDirectory(kernelDirectory);
}

void selectPageDirectory(struct PageDirectory *dir){
   currentDirectory = dir;
   asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
   uint32_t cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000;
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}

struct Page *getPage(uint32_t addr, bool make, struct PageDirectory *dir){
   addr /= 0x1000;
   uint32_t table_idx = addr / 1024;
   if (dir->tables[table_idx]){
       return &dir->tables[table_idx]->pages[addr%1024];
   }else if(make){
       uint32_t tmp;
       dir->tables[table_idx] = (struct PageTable*)kmalloc_ap(sizeof(struct PageTable), &tmp);
       memset(dir->tables[table_idx], 0, 0x1000);
       dir->tablesPhysical[table_idx] = tmp | 0x7;
       return &dir->tables[table_idx]->pages[addr%1024];
   }
   else
   {
       return 0;
   }
}