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
@              bump _kernel_malloc grows UP toward Bottom (away from stack)
@   0x03007CA0 ── FreeRamSpaceBottom / user SP  (1 312 B free)
@   0x03007CA0 – 0x03007FA0   user/sys stack   (USED — grows down)
@   0x03007FA0 ── IRQ SP
@   0x03007FA0 – 0x03008000   IRQ stack        (USED)
@   0x03007FF8 / 0x03007FFC   bootstrap / IRQ words (not free)
@   0x03008000  end of IWRAM
@
@ Safe leftover for custom code: ONLY 0x03007780–0x03007CA0 via _kernel_malloc.
@ Grow from Top upward so the first byte is NOT under the user SP.
@ Full unknown inventory: ram_map_iwram_pool.inc (431 gUnk_* symbols).
@ =============================================================================

SET_DATA FreeRamSpaceTop, 0x03007780
SET_DATA FreeRamSpaceBottom, 0x03007CA0
SET_DATA UsedFreeRamSpaceTop, FreeRamSpaceTop

SET_DATA gUserStackTop, 0x03007CA0
SET_DATA gIrqStackTop, 0x03007FA0

@ Last pool-backed vanilla global (82 hits). Free pool starts after pad.
@ Alias of gGaxWorkspacePtr (same word); kept for older docs / occupancy notes.
SET_DATA gVanillaIwramHighWater, 0x0300775C

.macro _kernel_malloc name, size
    SET_DATA \name, UsedFreeRamSpaceTop
    .set UsedFreeRamSpaceTop, UsedFreeRamSpaceTop + \size
.endm

@ -- GAX Sound Engine ----------------------------------------------------------

@ GAX2_PARAMS (~0x40 B); flags halfword at +0x0E (GAX_SPEECH = 0x40).
SET_DATA gGaxParams, 0x030008C0
@ Current song module pointer latch (PlayBgm dedup).
SET_DATA gGaxCurrentSong, 0x030008B0
@ Mix / workspace arena at vanilla site (IWRAM). Size 0x1000; ends at gOamCursor.
SET_ARRAY gGaxMixBuffer, 0x03005910, 0x1000
@ GAX_SPEECH object in uncharted IWRAM gap (0x03004348.. before 0x030050F0).
@ Must NOT be carved from the mix buffer (that overflows into gOamCursor).
SET_ARRAY gGaxSpeechObject, 0x03004348, 0x7A8
@ Pointer to GAX workspace (written by gax2_init).
SET_DATA gGaxWorkspacePtr, 0x0300775C
@ Current music volume (u16); PlayBgm reapplies via gax music-vol.
SET_DATA gGaxMusicVol, 0x030011FC

@ -- Engine / mode -------------------------------------------------------------

@ Main-loop mode byte (dispatch table @ ROM 0xBC04).
SET_DATA gMode, 0x03001630
@ Current actor index while AI / death-drop helpers run.
SET_DATA gCurrentActorIndex, 0x03001634

@ Pointer to the active 2D-flight player object.
SET_DATA gPlayerPtr, 0x03000DB8

@ Current EXP gem object while ExpGemUpdate runs (pos at +0x18/+0x1C).
SET_DATA gExpGemPtr, 0x03006FE0

@ Actor pool base (stride 0x60; flight index 0 = ship; also used by cutscenes).
SET_DATA gActorPool, 0x03002780
@ First index SpawnActor scans for a free slot; DeleteActor soft-clears below it.
SET_DATA gActorAllocIndex, 0x0300368C
@ Shared cutscene counter / spawn gate (also gUnk_03003688 in pool inventory).
SET_DATA gCutsceneCounter, 0x03003688

@ Soft OAM shadow (128 × 8 bytes) — DMA'd to 0x07000000 each VBlank.
SET_DATA gSoftOam, 0x03001F70
SET_DATA gOamCursor, 0x03006910
SET_DATA gSoftOamCount, 0x03006918
SET_DATA gAffineSlotCounter, 0x03006914
SET_DATA gActorOamStart, 0x03000D60
SET_DATA gUseYSorting, 0x03001658
@ Camera array (stride 0x84; X @ +0x2C, Y @ +0x30, both 16.16).
SET_DATA gCameras, 0x030009C0
@ Soft display mirrors — VBlank copies these to IO (see SetMode @ 0x080079F0).
SET_DATA gSoftDispCnt, 0x03000BF4
@ VBlank affine batch @ 0x08003AC4 writes these to REG_BG2PA / REG_BG2PB (dest
@ starts at 0x04000020), gated on the affine-mode byte @ 0x03000C4C. They are
@ NOT BG0CNT/BG1CNT mirrors: BGxCNT is written straight to IO by the camera
@ layer setup @ 0x080077D2 and never shadowed.
SET_DATA gSoftBg2Pa, 0x03001E7C
SET_DATA gSoftBg2Pb, 0x03001F6C
@ Authoritative DISPCNT shadow owned by the status/HUD display module. It is
@ re-applied to REG_DISPCNT every frame, so it — not gSoftDispCnt — decides
@ which layers are visible (see 0x08010B30 cluster).
SET_DATA gDisplayCtrlMirror, 0x03007194
@ HudSync @ 0x08010E58 early-outs when this is 0 (no HUD tilemap rebuild).
SET_DATA gHudEnabled, 0x0300715C
SET_DATA gStatusMenuOpen, 0x030076C4
@ Soft debug text map (32×22 halfwords) + dirty flags byte (bit 8 = text).
SET_ARRAY gSoftTextMap, 0x030050F0, 0x580
SET_DATA gSoftTextDirty, 0x03000C9C
@ Soft camera scroll mirrors written by UpdateCameras @ 0x08008F50.
SET_DATA gCamScrollMirrorX, 0x03000CF8
SET_DATA gCamScrollMirrorY, 0x03001E70
@ DrawDebugText / PresentDebugText latch (non-zero → text pending).
SET_DATA gDebugTextActive, 0x0300692C
@ Save busy latch / active slot (WriteSave @ 0x0800EC10).
SET_DATA gSaveBusy, 0x030070BC
SET_DATA gSaveSlot, 0x030070C0

@ -- Dialogue / cutscene -------------------------------------------------------

@ Runtime copy of the 7 talk-bank base pointers (InitTalkBanks @ 0x109A0).
SET_DATA gDialogueBankBases, 0x03000018
@ Per-frame cutscene step / case index (Ch.1 opener @ 0x523EC, etc.).
SET_DATA gCutsceneStep, 0x03007704
@ Cutscene lerp pair [0]=primary, [1]=secondary (paired with gCutsceneStep).
SET_ARRAY gCutsceneParam, 0x03007708, 0x8
@ Outer case index for stage FSM @ 0x0802B18C (JT; case 0 → talk id 109).
SET_DATA gStageCase, 0x03007738
@ Halfword gate used by stage intro case 268 (wait == 0x1F00).
SET_DATA gCutsceneGateHw, 0x030038A8
@ World / cutscene scroll registers (16.16; moved alongside gCameras).
SET_DATA gWorldScrollX, 0x03000C44
SET_DATA gWorldScrollY, 0x03000C6C
@ Case 62 waveform output (160 u16 entries).
SET_ARRAY gCutsceneWaveOutput, 0x030036A0, 0x140
@ Case 62 waveform phase byte.
SET_DATA gCutsceneWavePhase, 0x030076CC

@ Talk UI (StartTalkPtr @ 0x10808). Base block; flags halfword at +14.
SET_DATA gTalkState, 0x03007100
SET_DATA gTalkStreamPtr, 0x03007198
SET_DATA gTalkParamA, 0x03007184
SET_DATA gTalkParamB, 0x030070F4
SET_DATA gTalkBusyA, 0x030070F0
SET_DATA gTalkBusyB, 0x030070E8
SET_DATA gTalkParamAMirror, 0x03007154
SET_DATA gTalkLayout, 0x030071B4
SET_DATA gTalkPagePos, 0x030071D0
SET_DATA gTalkPagePos2, 0x030071F4
SET_DATA gTalkLayoutCmp, 0x030071B8
SET_DATA gTalkActive, 0x030070E0
SET_DATA gTalkGate, 0x03000014
SET_DATA gTalkExtra, 0x0300001C
@ TalkAdvance portrait header mirrors (speaker / side / expr).
SET_DATA gTalkPortraitSide, 0x0300001D
SET_DATA gTalkPortraitExpr, 0x0300001E

@ -- Overworld encounters / input ----------------------------------------------

@ Dedup latch for ScanEncounters mode 0 (last walk-into battle ID).
SET_DATA gLastEncounterBattleId, 0x03000040
@ Currently held keys (level-triggered; KEY_A=1, KEY_B=2, START=8, …).
SET_DATA gKeysHeld, 0x03001638
@ Newly pressed keys this frame (edge; written by key poll @ 0x3EB8).
SET_DATA gHeldKeys, 0x03002774
@ Pointer to map-object array (16-byte records).
SET_DATA gMapObjBasePtr, 0x03006FF0
@ Count of indices in gEncounterObjIndices.
SET_DATA gEncounterObjCount, 0x03007034
@ Player's own map-object index (byte).
SET_DATA gPlayerMapObjIndex, 0x0300704C
@ Indices of encounter-capable map objects (byte each).
SET_DATA gEncounterObjIndices, 0x03007070

@ -- Player RPG / flight combat state ------------------------------------------

SET_DATA gPlayerBombs, 0x03007014
SET_DATA gPlayerLevel, 0x0300702C
SET_DATA gPlayerExp, 0x0300705C
@ Status / flight HUD display mirror of gPlayerExp (HudSync @ 0x080110B6).
@ Status stats page draws (*gPlayerExpDisplay - gExpToCurrentLevel).
SET_DATA gPlayerExpDisplay, 0x03007144
@ Flight HUD decimal (enemy/quota remaining). NOT ship HP — vanilla stores 6/13/30/…
SET_DATA gEnemyRemaining, 0x03007080
SET_DATA gExpToCurrentLevel, 0x0300717C
SET_DATA gExpToNextLevel, 0x030071F0

@ Stage-clear latch checked by planet flight handlers / TriggerStageClear.
SET_DATA gStageClearFlag, 0x03007668
@ Overworld random-encounter cooldown (TryStartRandomBattle @ 0x1DA5C).
@ Non-zero → decrement and skip. CB "no random battles" forces byte @ +1 = 1.
SET_DATA gRandomBattleCooldown, 0x03007684
@ Event / tool ownership bitfield (GetFlag/SetFlag). CB "all tools" fills
@ halfwords that cover flags 31, 59, 88, 118, 155.
SET_DATA gEventFlags, 0x03000020
@ Currently selected overworld tool (0..5).
SET_DATA gEquippedTool, 0x03007064
@ Clear-sequence gate: 1 while battle active, 0 allows clear (CB Clear Stage).
SET_DATA gStageClearGate, 0x03007730

@ -- Inventory / Gun Data ------------------------------------------------------

@ Active loadout block (cannon @+0, bullet @+4, impact @+8; alt impact @+0x6C).
SET_DATA gGunLoadout, 0x03000720
SET_DATA gGunLoadoutBullet, 0x03000724
SET_DATA gGunLoadoutImpact, 0x03000728
SET_DATA gGunLoadoutImpactAlt, 0x0300078C
@ Non-zero → use primary impact at +8; zero → alt at +0x6C.
SET_DATA gGunLoadoutPrimaryFlag, 0x030007EE

@ Owned Gun Data bit array (IDs 1..77 with Suction), packed u32 words.
SET_DATA gGunDataBits, 0x03007690

@ Per-type ownership words (bits 0..N-1).
SET_DATA gBulletOwned, 0x0300773C
SET_DATA gItemsOwned, 0x03007740
SET_DATA gCannonOwned, 0x03007744
SET_DATA gImpactOwned, 0x03007748

@ Flight shot aux pool (stride 0x40; hitbox at +0x10/+0x12).
SET_DATA gShotAuxPool, 0x030004A0
@ AllocShotSlot ring counter (vanilla wraps at 10).
SET_DATA gShotSlotCounter, 0x030002F0

@ -- Pool-backed unknowns (auto-generated) -------------------------------------
.include "ram_map_iwram_pool.inc"

@ -- Custom free-space allocations ---------------------------------------------
@ Prefer _kernel_malloc here for small hot-path scratch. Grow upward from Top.
@ NEVER use C `static` locals in APPEND_TEXT — the linker puts .bss at
@ 0x03000000 and will stomp vanilla IWRAM (event flags / Recker form / etc.).

@ Phoenix Impact: 1 = revive already used this battle.
_kernel_malloc gPhoenixReviveUsed, 0x1
@ Magic bytes initialize the revive flag even if this free RAM starts dirty.
_kernel_malloc gPhoenixReviveMagicA, 0x1
_kernel_malloc gPhoenixReviveMagicB, 0x1
@ Set for the caller tail after death FX so Phoenix can skip player deletion.
_kernel_malloc gPhoenixReviveTailSkips, 0x1
@ Durable No$GBA print scratch (must outlive the call; not stack / not .bss).
_kernel_malloc gNoCashPrintBuf, 0x100
@ One-shot latches formerly wrongly placed in .bss @ 0x03000000+.
_kernel_malloc gInventoryCheatsApplied, 0x1
_kernel_malloc gNoCashHeartbeat, 0x1
@ Edge-trigger crash TTY logs (clear when ship is no longer crashed).
_kernel_malloc gPhoenixCrashLogged, 0x1
@ When set, GetArchiveFileStart(0x71) returns the REVIVE popup ANM.
_kernel_malloc gPhoenixReviveAnmSwap, 0x1
@ Frames left before restoring the "+ EXP" ANM bank after a REVIVE popup.
_kernel_malloc gPhoenixReviveAnmRestore, 0x1
@ Pad so gActorMaxHp is word-aligned.
_kernel_malloc gActorMaxHpAlignPad, 0x3
@ Per-slot max HP snapshot from InitActorParams (40 × u32).
_kernel_malloc gActorMaxHp, 0xA0
@ 1 = HP-bar tiles queued into OBJ VRAM this session.
_kernel_malloc gHpBarTilesReady, 0x1
@ Pad so gHpBarTileScratch is word-aligned (u32 encode + DMA src).
_kernel_malloc gHpBarTileScratchAlignPad, 0x3
@ Scratch for one-time bar tile encode (9 × 2 × 32 = 0x240).
_kernel_malloc gHpBarTileScratch, 0x240
@ 1 = custom GAX speech table installed into workspace object.
@ (moved: gGaxSpeechInstalled is derived from the workspace at runtime — the
@ free pool overlaps user-stack headroom and gets thrashed; see gax-audio.md)
@ Charge Shot rework: 0 = charging, 1 = empowered (10x window).
_kernel_malloc gChargeShotPhase, 0x1
@ Frames elapsed in the current charge / empowered phase.
_kernel_malloc gChargeShotTimer, 0x2
