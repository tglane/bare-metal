SECTIONS {
    . = 0x1000;

	.text : SUBALIGN(4)
	{
        *(.text.entry)      /* Ensure that this area appears first in the niary */
		*(.text)
        *(.rodata)
        *(.data)
	}
    
    .rodata :
    {
    	*(.rodata)
    }

    .data :
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
    /*__bss_end = .;
    __bss_sizeb = __bss_end - __bss_start;
    __bss_sizel = (__bss_end - _bss_start) / 4;*/

    /DISCARD/ : 
    {
        *(.eh_frame);
        *(.comment);
    }

    end = .; _end = .; __end = .;
}
