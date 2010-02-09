MEMORY
{
   FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
   SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}
SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		*(.text*)
		*(.rodata*)
		. = ALIGN(8);
		_etext = .;
        
	} >FLASH
	
	/* .ARM.exidx is sorted, so has to go in its own output section. */
	__exidx_start = .;
	.ARM.exidx :
 	{
    	*(.ARM.exidx* .gnu.linkonce.armexidx.*)
  	} >FLASH
 	__exidx_end = .;
	
	_flash_data = .;
    .data : AT (_flash_data)
    {
       _data = .;
       *(vtable)
       *(.data*)
       _edata = .;
    } > SRAM
    .bss :
    {
       _bss = .;
       *(.bss*)
       *(COMMON)
       _ebss = .;
       _end = .;
    __end = .;
    } > SRAM
}

/* end of allocated ram _end */
PROVIDE( _HEAP_START = _end );

/* end of the heap -> align 8 byte */
PROVIDE ( _HEAP_END = ALIGN(ORIGIN(SRAM) + LENGTH(SRAM) - 8 ,8) ); 

