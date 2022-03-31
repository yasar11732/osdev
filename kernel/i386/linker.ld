
ENTRY(_early_init)

SECTIONS {
    . = 1M;

    .multiboot ALIGN (4): 
    {
        *(.multiboot)
    }

    .earlyinit ALIGN (4):
    {
        *(.earlyinit)
    }

    . += 0xC0000000;
    _kernel_start = .;
    .text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000)
    {
        _text_section_start = .;
        *(.text)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000)
    {
        _rodata_section_start = .;
        *(.rodata)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000)
    {
        _data_section_start = .;
        *(.data)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000)
    {
        _bss_section_start = .;
        *(COMMON)
        *(.bss)
    }

    .pagestructs ALIGN(4K) : AT(ADDR(.pagestructs) - 0xC0000000)
    {
        *(.pagestructs)   
    }

    _kernel_end = ALIGN(4096);
}