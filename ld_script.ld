OUTPUT_ARCH(arm)
ENTRY(gBaserom)

MEMORY {
    EWRAM (rwx) : ORIGIN = 0x02000000, LENGTH = 256K
    IWRAM (rwx) : ORIGIN = 0x03000000, LENGTH = 32K
    /* 16M: 8M baserom + append space for src_custom / runtime config */
    ROM   (rx)  : ORIGIN = 0x08000000, LENGTH = 16M
}

/* Absolute IWRAM addresses for peeled symbols */
gPlayerLevel = 0x0300702C;
gPlayerExp = 0x0300705C;
gExpToCurrentLevel = 0x0300717C;
gExpToNextLevel = 0x030071F0;

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

    /* Named .rom so orphan .text (libgcc) does not extend the baserom image. */
    .rom :
    {
        asm/rom.o(.rodata)
        src/level_up.o(.text)
        asm/rom_after_a.o(.rodata)
        asm/flight_skip.o(.text)
        asm/rom_after_b.o(.rodata)
        . = ALIGN(4);
    } > ROM = 0xFF

    /* Custom code past the 8MB retail image (ygodm8-style append). */
    __append_start = .;

    .runtime_config :
    {
        configs/runtime.o(.runtime_config);
    } > ROM = 0xFF

    .append_text :
    {
        src_custom/*.o(.text);
        *(.text);
        *(.text.*);
        *(.append_text);
    } > ROM = 0xFF

    .append_rodata :
    {
        src_custom/*.o(.rodata);
        *(.rodata);
        *(.rodata.*);
        *(.append_rodata);
    } > ROM = 0xFF

    .append_data :
    {
        *(.data);
        *(.append_data);
    } > ROM = 0xFF

    . = ALIGN(4);
    __append_end = .;

    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.ARM.attributes)
    }
}
