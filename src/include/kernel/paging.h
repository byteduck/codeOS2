#ifndef PAGING_H
#define PAGING_H

extern long kstart;
extern long kend;
extern void load_page_dir(unsigned int*);
void setupPaging();
void pageFaultHandler(struct registers *r);
void exec(uint8_t* prog);

#endif