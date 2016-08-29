extern long kstart;
extern long kend;
extern void load_page_dir(unsigned int*);
void setupPaging();
void pageFaultHandler(struct registers *r);
#include "paging.c"