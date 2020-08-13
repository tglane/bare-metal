SECTIONS {
    . = 0x1000;

	.text : SUBALIGN(4)
	{
        *(.text.entry)      /* Ensure that this area appears first in the niary */
		*(.text)
        *(.rodata)
        *(.data)
	}
    
    .rodata : SUBALIGN(4)
    {
    	*(.rodata)
    }

    .data : SUBALIGN(4)
    {
    	*(.data)
    }

    .bss : SUBALIGN(4)
    {
        /*__bss_start = .;*/
        *(COMMON)
    	*(.bss)
    }
    . = ALIGN(4);

    /DISCARD/ : 
    {
        *(.eh_frame);
        *(.comment);
    }

    end = .; _end = .; __end = .;
}
