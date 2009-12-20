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
        _etext = .;
    } > FLASH
    
    .data : AT (_etext + 8)
    /* The offset of 8 Bytes is a workaround for the CodeSourcery C++ Compiler.
    also defined in the startup.c file */
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
        . = ALIGN (8);
 		_end = .;
        
    } > SRAM
    
    PROVIDE(__HEAP_START = _end );
    
}
