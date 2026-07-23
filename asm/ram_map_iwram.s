@ =============================================================================
@ IWRAM occupancy map (baserom Thumb LDR literal-pool scan)
@ =============================================================================
@ Hardware window:     0x03000000 – 0x03008000   (32 KiB)
@ Rescan: python3 tools/scan_ram_literals.py [--emit-asm]
@
@ Pool-backed word-aligned clusters (merge gap < 0x40), last hit = high water:
@   0x03000000–0x03000050   0x03000150
@   0x03000200–0x03000280   0x030002CC–0x030002F8
@   0x03000398  0x03000410  0x030004A0  0x03000720–0x0300072C
@   0x030007E8–0x03000A28   0x03000BD0–0x03000DD0   (dense; gPlayerPtr cluster)
@   0x030011D0–0x03001210   0x03001420
@   0x03001620–0x03001670   (gMode cluster)
@   0x03001E70–0x03001ED0   0x03001F50–0x03001F70
@   0x03002370  0x03002770–0x030027E0  0x03002B40
@   0x03003680–0x030036A0   0x03003704  0x0300381C  0x030038A0–0x030038F0
@   0x03003FF0  0x03004344  0x030050F0  0x03005708  0x030058F0–0x03005910
@   0x03006910–0x03006960
@   0x03006FE0–0x03007220   (player RPG / flight HUD)
@   0x03007420–0x03007460   0x03007570
@   0x03007660–0x0300775C   (inventory / stage-clear — VANILLA HIGH WATER)
@
@   0x0300775C – 0x03007780   pad (do not touch)
@   0x03007780 ── FreeRamSpaceTop ──────────── SAFE custom pool (FREE)
@              bump _kernel_malloc grows downward toward Top
@   0x03007CA0 ── FreeRamSpaceBottom / user SP  (1 312 B free)
@   0x03007CA0 – 0x03007FA0   user/sys stack   (USED — grows down)
@   0x03007FA0 ── IRQ SP
@   0x03007FA0 – 0x03008000   IRQ stack        (USED)
@   0x03007FF8 / 0x03007FFC   bootstrap / IRQ words (not free)
@   0x03008000  end of IWRAM
@
@ Safe leftover for custom code: ONLY 0x03007780–0x03007CA0 via _kernel_malloc.
@ Full unknown inventory: ram_map_iwram_pool.inc (431 gUnk_* symbols).
@ =============================================================================

SET_DATA FreeRamSpaceTop, 0x03007780
SET_DATA FreeRamSpaceBottom, 0x03007CA0
SET_DATA UsedFreeRamSpaceTop, FreeRamSpaceBottom

SET_DATA gUserStackTop, 0x03007CA0
SET_DATA gIrqStackTop, 0x03007FA0

@ Last pool-backed vanilla global (82 hits). Free pool starts after pad.
SET_DATA gVanillaIwramHighWater, 0x0300775C

.macro _kernel_malloc name, size
    .set UsedFreeRamSpaceTop, UsedFreeRamSpaceTop - \size
    SET_DATA \name, UsedFreeRamSpaceTop
.endm

@ -- Engine / mode -------------------------------------------------------------

@ Main-loop mode byte (dispatch table @ ROM 0xBC04).
SET_DATA gMode, 0x03001630

@ Pointer to the active 2D-flight player object.
SET_DATA gPlayerPtr, 0x03000DB8

@ -- Player RPG / flight combat state ------------------------------------------

SET_DATA gPlayerBombs, 0x03007014
SET_DATA gPlayerLevel, 0x0300702C
SET_DATA gPlayerExp, 0x0300705C
@ Flight HUD decimal (enemy/quota remaining). NOT ship HP — vanilla stores 6/13/30/…
SET_DATA gEnemyRemaining, 0x03007080
SET_DATA gExpToCurrentLevel, 0x0300717C
SET_DATA gExpToNextLevel, 0x030071F0

@ Stage-clear latch checked by planet flight handlers / TriggerStageClear.
SET_DATA gStageClearFlag, 0x03007668
@ Clear-sequence gate: 1 while battle active, 0 allows clear (CB Clear Stage).
SET_DATA gStageClearGate, 0x03007730

@ -- Inventory / Gun Data ------------------------------------------------------

@ Owned Gun Data bit array (IDs 1..76), packed u32 words.
SET_DATA gGunDataBits, 0x03007690

@ Per-type ownership words (bits 0..N-1).
SET_DATA gBulletOwned, 0x0300773C
SET_DATA gItemsOwned, 0x03007740
SET_DATA gCannonOwned, 0x03007744
SET_DATA gImpactOwned, 0x03007748

@ -- Pool-backed unknowns (auto-generated) -------------------------------------
.include "ram_map_iwram_pool.inc"

@ -- Custom free-space allocations ---------------------------------------------
@ Prefer _kernel_malloc here for small hot-path scratch. Grow downward only.
