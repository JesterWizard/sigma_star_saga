@ =============================================================================
@ Save / SRAM-bus occupancy map (baserom Thumb LDR literal-pool scan)
@ =============================================================================
@ Cart save: EEPROM_V124 (marker string @ ROM 0x24BF44) — NOT mapped SRAM.
@ Vanilla I/O goes through the EEPROM library; do not treat 0x0E… as save RAM.
@ Rescan: python3 tools/scan_ram_literals.py
@
@ GBA SRAM bus window (hardware): 0x0E000000 – 0x0E010000  (64 KiB mirror)
@ Pool-backed aligned hits in baserom (all single-hit — treated as false
@ positives in graphics/data, not vanilla save globals):
@   0x0E000F90  (1)   0x0E009F00  (1)
@ Unaligned pool words also appear (0x0E003FBB, 0x0E00AA09) — ignore.
@
@   0x0E000000 ── FreeFlashSpaceTop ────────── opt-in scratch (FREE**)
@              bump _kernel_malloc_flash_free grows downward toward Top
@   0x0E000100 ── FreeFlashSpaceBottom         (256 B reserved here)
@   0x0E000100 – 0x0E010000   rest of bus     (UNUSED by this map)
@
@ ** Opt-in only. Keep tiny; verify on hardware/mGBA before relying on it.
@ Real persistence = EEPROM. No gUnk inventory (no trustworthy pool targets).
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
