ENTRY(start)

SECTIONS {
    . = 1M;

	krnlstart = .;

    .text :
    {
        *(.text)
    }
	
	.data :
	{
		*(.data)
	}
	
	.bss :
	{
		*(COMMON)
		*(.bss)
	}
	
	krnlend = .;
}