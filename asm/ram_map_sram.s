@ =============================================================================
@ Save storage (EEPROM) / unused SRAM bus window
@ =============================================================================
@ Cart save chip: EEPROM_V124 (string @ ROM 0x24BF44). Vanilla persistence
@ goes through the EEPROM library, not a mapped SRAM/Flash image at 0x0E…
@
@ Absolute 0x0E000000 literals are abundant in the baserom but mostly false
@ positives in graphics/data. Do not treat the GBA SRAM window as save space
@ unless a future hack explicitly remaps or patches save I/O.
@
@ Symbols below reserve a documented scratch window on the SRAM bus for any
@ future tooling that needs a fixed 0x0E address. Keep allocations tiny and
@ verify on hardware/emulator that the cart mirror behaves as expected.
@ =============================================================================

SET_DATA FreeFlashSpaceTop, 0x0E000000
SET_DATA FreeFlashSpaceBottom, 0x0E000100
SET_DATA UsedFreeFlashSpaceTop, FreeFlashSpaceBottom

.macro _kernel_malloc_flash_free name, size
    .set UsedFreeFlashSpaceTop, UsedFreeFlashSpaceTop - \size
    SET_DATA \name, UsedFreeFlashSpaceTop
.endm

@ Marker only — documents the EEPROM library identity in the baserom.
SET_DATA gEepromLibraryMarker, 0x0824BF44

@ -- Custom allocations (unused by vanilla; opt-in for experiments) ------------
@ _kernel_malloc_flash_free gExampleSramScratch, 0x10
