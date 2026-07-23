@ =============================================================================
@ EWRAM occupancy map (baserom Thumb LDR literal-pool scan)
@ =============================================================================
@ Hardware window:     0x02000000 – 0x02040000   (256 KiB)
@ Rescan: python3 tools/scan_ram_literals.py [--emit-asm]
@
@ Init DMA-clears the whole region. High absolute 0x02… literals in ROM are
@ often art/compressed-data false positives — only pool-backed aligned hits
@ with count>=2 are inventoried below FreeEwramSpaceTop.
@
@ Dense pool-backed clusters (merge gap < 0x100):
@   0x02000000–0x02000010   0x020003C0–0x02000420   0x02000600
@   0x02000820–0x02000880   0x02000A80–0x02000B00
@   0x02000D00–0x02000D10   0x02000F10–0x02000F20
@   0x02001E20–0x02001E40   0x02002040–0x02002070   0x02002188
@   0x02002470–0x02002570   0x02002970–0x020029A0
@   0x02002DA0–0x02002E90   0x020030A0–0x020030C0   0x02003A00
@   0x020050C0–0x02005130   0x02005930–0x02005940
@   0x02005D40–0x02005E00   ← densest upper confirmed use
@
@ Single-hit / unaligned pool words above ~0x02005E00 (0x0200FC1C, 0x02021514,
@ 0x020302E0, 0x02035620, …) sit in graphics-looking ROM — treated as suspects,
@ NOT as reason to raise FreeEwramSpaceTop.
@
@   0x02030000 ── FreeEwramSpaceTop ────────── SAFE custom pool (FREE*)
@              bump _kernel_malloc_ewram grows downward toward Top
@   0x02040000 ── FreeEwramSpaceBottom         (64 KiB free)
@
@ * Floor remains provisional/conservative. Dense vanilla use ends ~0x02005E00;
@   do not lower FreeEwramSpaceTop without stronger evidence.
@ Safe leftover: ONLY 0x02030000–0x02040000 via _kernel_malloc_ewram*.
@ Inventory: ram_map_ewram_pool.inc (142 gUnk_* symbols, hits>=2).
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

@ -- Pool-backed unknowns (auto-generated) -------------------------------------
.include "ram_map_ewram_pool.inc"

@ -- Custom free-space allocations ---------------------------------------------
@ Prefer _kernel_malloc_ewram / _kernel_malloc_ewram_array for larger buffers.
