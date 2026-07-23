OUTPUT_ARCH(arm)
ENTRY(gBaserom)

MEMORY {
    EWRAM (rwx) : ORIGIN = 0x02000000, LENGTH = 256K
    IWRAM (rwx) : ORIGIN = 0x03000000, LENGTH = 32K
    ROM   (rx)  : ORIGIN = 0x08000000, LENGTH = 8M
}

SECTIONS {
    ewram (NOLOAD) : ALIGN(4)
    {
        __ewram_start = .;
        *(ewram_data)
        . = ALIGN(4);
        __ewram_end = .;
    } > EWRAM

    iwram (NOLOAD) : ALIGN(4)
    {
        __iwram_start = .;
        *(.bss)
        *(COMMON)
        *(iwram_data)
        . = ALIGN(4);
        __iwram_end = .;
    } > IWRAM

    .text : ALIGN(4)
    {
        /* Scaffold stage: whole ROM is one blob at 0x08000000.
           Later insert crt0 / C objects here and shrink asm/rom.s. */
        asm/rom.o(.rodata)
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > ROM = 0xFF

    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.ARM.attributes)
    }
}
