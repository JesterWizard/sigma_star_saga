@ =============================================================================
@ IWRAM
@ =============================================================================
@ Full window: 0x03000000–0x03008000 (32 KiB)
@
@ Stacks (from crt0 @ ROM 0xF4 / 0xF8):
@   User/sys SP  = 0x03007CA0
@   IRQ SP       = 0x03007FA0
@
@ Free pool for custom allocations grows downward from the user stack:
@   0x03007780–0x03007CA0
@ Last heavily-referenced vanilla cluster ends near 0x0300775C; leave a pad
@ before FreeRamSpaceTop. Do not allocate into the stack region.
@ =============================================================================

SET_DATA FreeRamSpaceTop, 0x03007780
SET_DATA FreeRamSpaceBottom, 0x03007CA0
SET_DATA UsedFreeRamSpaceTop, FreeRamSpaceBottom

SET_DATA gUserStackTop, 0x03007CA0
SET_DATA gIrqStackTop, 0x03007FA0

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

@ -- Custom free-space allocations ---------------------------------------------
@ Prefer _kernel_malloc here for small hot-path scratch. Grow downward only.
