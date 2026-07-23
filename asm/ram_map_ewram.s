@ =============================================================================
@ EWRAM
@ =============================================================================
@ Full window: 0x02000000–0x02040000 (256 KiB)
@
@ Vanilla use is still sparsely labeled. Init DMA-clears the whole region.
@ High absolute literals in the baserom are often false positives from art/
@ compressed data; prefer symbols discovered via code xrefs.
@
@ Provisional free pool (upper EWRAM, bump allocator grows downward):
@   0x02030000–0x02040000
@ Revisit FreeEwramSpaceTop as more vanilla EWRAM globals are matched.
@ =============================================================================

SET_DATA FreeEwramSpaceTop, 0x02030000
SET_DATA FreeEwramSpaceBottom, 0x02040000
SET_DATA UsedFreeEwramSpaceTop, FreeEwramSpaceBottom

.macro _kernel_malloc_ewram name, size
    .set UsedFreeEwramSpaceTop, UsedFreeEwramSpaceTop - \size
    .set UsedFreeEwramSpaceTop, UsedFreeEwramSpaceTop & ~3
    SET_DATA \name, UsedFreeEwramSpaceTop
.endm

.macro _kernel_malloc_ewram_array name, size
    .set UsedFreeEwramSpaceTop, UsedFreeEwramSpaceTop - \size
    .set UsedFreeEwramSpaceTop, UsedFreeEwramSpaceTop & ~3
    SET_ARRAY \name, UsedFreeEwramSpaceTop, \size
.endm

@ -- Known / noted EWRAM anchors (expand as decomp peels) ----------------------
@ 0x02000000 appears throughout the ROM (often non-pointer data). Treat new
@ SET_DATA entries as provisional until confirmed by Thumb literal-pool xrefs.

@ -- Custom free-space allocations ---------------------------------------------
@ Prefer _kernel_malloc_ewram / _kernel_malloc_ewram_array for larger buffers.
