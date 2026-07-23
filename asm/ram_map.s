.section .rodata

@ =============================================================================
@ RAM map entry — assembles region fragments into one object (build/sss/asm/ram_map.o)
@ =============================================================================
@
@ Region sources (edit these, not this file, when adding symbols):
@   ram_map_iwram.s  — IWRAM  (free pool 0x03007780–0x03007CA0)
@   ram_map_ewram.s  — EWRAM  (free pool 0x02030000–0x02040000, provisional)
@   ram_map_sram.s   — EEPROM note + unused 0x0E000000 window
@
@ Fragments are .included here (not compiled separately) so bump-allocator
@ cursors carry across regions in a single assembly pass.
@ Pattern: https://github.com/JesterWizard/ygodm8/blob/master/asm/ram_map.s
@ Guide: documentation/ram-map.md
@ =============================================================================

.macro SET_DATA name, value
    .global \name
    .type \name, object
    .set \name, \value
.endm

.macro SET_ARRAY name, value, size
    SET_DATA \name, \value
    .global \name\()End
    .type \name\()End, object
    .set \name\()End, (\value + \size)
.endm

.macro dat value, name
    SET_DATA \name, \value
.endm

.include "ram_map_iwram.s"
.include "ram_map_ewram.s"
.include "ram_map_sram.s"
