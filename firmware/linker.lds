ENTRY( _start )

PROVIDE( __stack_size = 512 );

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K
    RAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 4K
}

SECTIONS
{
    .vectors :
    {
        KEEP (*(.vectors));
        . = ALIGN(4);
    } >FLASH
    
    .text :
    {
        *(.text)
        *(.text.*)
        *(.gnu.linkonce.t.*)
        *(.rodata)
        *(.rodata*)
        *(.gnu.linkonce.r.*)
        . = ALIGN(4);
    } >FLASH

    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH

    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH

    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } >FLASH
    __exidx_end = .;

    . = ALIGN(4);
    _etext = .;

    .data : ALIGN(4)
    {
        _data = .;
        *(.ramfunc .ramfunc.*);
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT>FLASH

    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        *(.gnu.linkonce.b.*)
        . = ALIGN(4);
        _ebss = .;
        PROVIDE(_end = .);
    } >RAM

    .stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size :
    {
        PROVIDE( _heap_end = . );
        PROVIDE( _susrstack = . );
        . = . + __stack_size;
        PROVIDE( _eusrstack = .);
    } >RAM
}
