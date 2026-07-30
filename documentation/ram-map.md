# RAM Map

## Introduction

`asm/ram_map.s` is the central address registry. It `.include`s three region sources that assemble into a single `ram_map.o`:

| File | Region |
|------|--------|
| `asm/ram_map_iwram.s` | IWRAM free pool + known vanilla symbols |
| `asm/ram_map_iwram_pool.inc` | Auto pool inventory (`gUnk_*`, regenerate via scanner) |
| `asm/ram_map_ewram.s` | EWRAM free pool (provisional) |
| `asm/ram_map_ewram_pool.inc` | Auto pool inventory (`gUnk_*`, hits≥2) |
| `asm/ram_map_sram.s` | EEPROM note + unused `0x0E000000` window |

Fragments are included (not separate objects) so bump-allocator cursors stay consistent. Pattern follows [ygodm8 `ram_map.s`](https://github.com/JesterWizard/ygodm8/blob/master/asm/ram_map.s).

Custom code should reference symbols from this map instead of hard-coding `0x03…` / `0x02…` addresses.

## Scanning the baserom

```bash
python3 tools/scan_ram_literals.py              # report clusters / top hits
python3 tools/scan_ram_literals.py --emit-asm   # refresh asm/ram_map_*_pool.inc
```

Method: Thumb `LDR Rd,[PC,#imm]` literal pools only (not raw absolute words). Word-aligned targets preferred; EWRAM requires hits≥2 below `FreeEwramSpaceTop`.

## Occupancy (used vs safe leftover)

| Region | Span | Size | Status |
|--------|------|------|--------|
| IWRAM hardware | `0x03000000`–`0x03008000` | 32 KiB | — |
| IWRAM vanilla (pool high water) | `0x03000000`–`0x0300775C` | — | **USED** (431+ named pool symbols) |
| IWRAM pad | `0x0300775C`–`0x03007780` | 36 B | leave alone |
| IWRAM custom free | `0x03007780`–`0x03007CA0` | **1 312 B** | **SAFE** via `_kernel_malloc` |
| IWRAM user stack | `0x03007CA0`–`0x03007FA0` | 768 B | **USED** (grows down) |
| IWRAM IRQ stack | `0x03007FA0`–`0x03008000` | 96 B | **USED** |
| EWRAM hardware | `0x02000000`–`0x02040000` | 256 KiB | — |
| EWRAM dense pool use | `0x02000000`–`~0x02005E00` | — | **USED** (142 `gUnk_*`) |
| EWRAM probe band | `~0x02005E00`–`0x0202B000` | — | canary probe dirty ≤`0x020071C0` (title→talk); left as unused margin |
| EWRAM custom free | `0x0202B000`–`0x02040000` | **~84 KiB** | **SAFE** via `_kernel_malloc_ewram*` |
| Save chip | EEPROM_V124 | — | Not `0x0E…` |
| SRAM-bus scratch** | `0x0E000000`–`0x0E000100` | 256 B | Opt-in only |

\* EWRAM free floor lowered to `0x0202B000` after `tools/mgba_ewram_canary_probe.c` (last dirty `0x020071C0`). Fits `gVoiceDecodeBuf` (80 KiB) + FX wave-set/package RAM.  
\*\* Not cart save; verify before use. Pool scan found no trustworthy SRAM-bus globals.

## Macros

- `SET_DATA` — fixed absolute symbol
- `SET_ARRAY` — span with `name` / `nameEnd`
- `_kernel_malloc` / `_kernel_malloc_ewram` — downward free-space bump allocators

## Known IWRAM symbols

| Symbol | Address | Role |
|--------|---------|------|
| `gMode` | `0x03001630` | Main-loop mode |
| `gCurrentActorIndex` | `0x03001634` | Current actor for AI / fauna death-drop |
| `gGunLoadout` | `0x03000720` | Active gun loadout block |
| `gGunLoadoutBullet` | `0x03000724` | Equipped bullet index |
| `gGunLoadoutImpact` | `0x03000728` | Primary equipped impact index |
| `gGunLoadoutImpactAlt` | `0x0300078C` | Alternate equipped impact index |
| `gGunLoadoutPrimaryFlag` | `0x030007EE` | Non-zero → primary impact |
| `gPlayerPtr` | `0x03000DB8` | Flight player object pointer |
| `gKeysHeld` | `0x03001638` | Currently held keys (level) |
| `gHeldKeys` | `0x03002774` | Newly pressed keys this frame (edge) |
| `gLastEncounterBattleId` | `0x03000040` | `ScanEncounters` mode-0 dedup latch |
| `gMapObjBasePtr` | `0x03006FF0` | Pointer to map-object array |
| `gEncounterObjCount` | `0x03007034` | Encounter object index count |
| `gPlayerMapObjIndex` | `0x0300704C` | Player map-object index |
| `gEncounterObjIndices` | `0x03007070` | Encounter object index list |
| `gExpGemPtr` | `0x03006FE0` | Current EXP gem during `ExpGemUpdate` |
| `gActorPool` | `0x03002780` | Actor pool (stride `0x60`; flight + cutscenes) |
| `gActorAllocIndex` | `0x0300368C` | `SpawnActor` free-slot scan start |
| `gCutsceneCounter` | `0x03003688` | Cutscene wait counter / spawn gate |
| `gSoftOam` | `0x03001F70` | Soft OAM shadow (128 × 8 bytes) |
| `gOamCursor` | `0x03006910` | Soft OAM write cursor |
| `gSoftOamCount` | `0x03006918` | Soft OAM entry count (128) |
| `gCameras` | `0x030009C0` | Camera array (stride `0x84`) |
| `gSoftDispCnt` | `0x03000BF4` | Soft `DISPCNT` (VBlank → `REG_DISPCNT`) |
| `gSoftBg2Pa` | `0x03001E7C` | Affine batch (VBlank → `REG_BG2PA`) |
| `gSoftBg2Pb` | `0x03001F6C` | Affine batch (VBlank → `REG_BG2PB`) |
| `gDisplayCtrlMirror` | `0x03007194` | Status/HUD `DISPCNT` shadow, re-applied every frame (overrides `gSoftDispCnt`) |
| `gHudEnabled` | `0x0300715C` | `HudSync` @ `0x08010E58` early-outs when `0` |
| `gStatusMenuOpen` | `0x030076C4` | SELECT status UI open latch |
| `gDialogueBankBases` | `0x03000018` | Runtime ptr to 7 talk-bank bases |
| `gDialogueEntryOffsets` | `0x02000010` | Per-script halfword offsets into banks |
| `gCutsceneStep` | `0x03007704` | Per-frame cutscene step / case index |
| `gCutsceneParam` | `0x03007708` | Cutscene lerp pair (8 bytes) |
| `gStageCase` | `0x03007738` | Stage FSM @ `0x0802B18C` outer JT index |
| `gCutsceneGateHw` | `0x030038A8` | Stage intro case wait halfword |
| `gWorldScrollX` | `0x03000C44` | World / cutscene scroll X (16.16) |
| `gWorldScrollY` | `0x03000C6C` | World / cutscene scroll Y (16.16) |
| `gCutsceneWaveOutput` | `0x030036A0` | Case 62 waveform output (160 halfwords) |
| `gCutsceneWavePhase` | `0x030076CC` | Case 62 waveform phase |
| `gTalkState` | `0x03007100` | Talk UI block (flags halfword at +14) |
| `gTalkStreamPtr` | `0x03007198` | Active talk stream cursor |
| `gTalkGate` | `0x03000014` | Talk gate latch |
| `gTalkExtra` | `0x0300001C` | `StartTalkByIdEx` latch / TalkAdvance speaker |
| `gTalkPortraitSide` | `0x0300001D` | TalkAdvance portrait side |
| `gTalkPortraitExpr` | `0x0300001E` | TalkAdvance portrait expr |
| `gPlayerBombs` | `0x03007014` | Smart bomb count |
| `gPlayerLevel` | `0x0300702C` | RPG level |
| `gPlayerExp` | `0x0300705C` | Experience (canonical / save) |
| `gPlayerExpDisplay` | `0x03007144` | Status/HUD EXP mirror (`HudSync` copies from `gPlayerExp`) |
| `gEnemyRemaining` | `0x03007080` | Flight HUD enemy/quota digit (not HP) |
| `gExpToCurrentLevel` | `0x0300717C` | Exp floor for current level |
| `gExpToNextLevel` | `0x030071F0` | Exp to next level |
| `gEventFlags` | `0x03000020` | Story/tool bitfield (`GetFlag` / `SetFlag`) |
| `gEquippedTool` | `0x03007064` | Active overworld tool index (0–5) |
| `gStageClearFlag` | `0x03007668` | Stage-clear latch |
| `gRandomBattleCooldown` | `0x03007684` | Overworld RNG encounter delay (`0x1DA5C`; CB writes `+1`) |
| `gStageClearGate` | `0x03007730` | Clear-sequence gate (0 = allow; CB Clear Stage) |
| `gGaxParams` | `0x030008C0` | GAX2_PARAMS (flags @ +0x0E; `GAX_SPEECH` = `0x40`) |
| `gGaxCurrentSong` | `0x030008B0` | PlayBgm song-module latch |
| `gGaxMixBuffer` | `0x03005910` | GAX mix arena (`0x1000` IWRAM; vanilla site) |
| `gGaxSpeechObject` | `0x03004348` | GAX_SPEECH object (`0x7A4` IWRAM; uncharted gap) |
| `gGaxWorkspacePtr` | `0x0300775C` | Pointer to mix workspace (same word as legacy high-water note) |
| `gGaxMusicVol` | `0x030011FC` | Music volume (u16) |
| `gGunDataBits` | `0x03007690` | Gun Data ownership bits |
| `gBulletOwned` | `0x0300773C` | Bullet Gun Data word |
| `gShotAuxPool` | `0x030004A0` | Flight shot aux (stride `0x40`, vanilla 10 slots) |
| `gShotSlotCounter` | `0x030002F0` | `AllocShotSlot` ring index |
| `gItemsOwned` | `0x03007740` | Key / overworld items |
| `gCannonOwned` | `0x03007744` | Cannon Gun Data word |
| `gImpactOwned` | `0x03007748` | Impact Gun Data word |
| `gVanillaIwramHighWater` | `0x0300775C` | Last pool-backed vanilla global |
| `gChargeShotPhase` | (free pool) | Charge Shot: 0 = charging, 1 = empowered |
| `gChargeShotTimer` | (free pool) | Frames in the current charge/empowered phase |
| `gSoftTextMap` | `0x030050F0` | Soft debug text map (32×22 halfwords) |
| `gSoftTextDirty` | `0x03000C9C` | Soft text dirty flags (bit 8 = text map) |
| `gCamScrollMirrorX` | `0x03000CF8` | Soft camera scroll X (`UpdateCameras`) |
| `gCamScrollMirrorY` | `0x03001E70` | Soft camera scroll Y (`UpdateCameras`) |
| `gSaveBusy` | `0x030070BC` | `WriteSave` busy latch |
| `gDebugMenuActive` | (EWRAM free pool) | 0=closed, 1=START debug menu open |
| `gDebugMenuTextState` | (EWRAM free pool) | String painted in the text map; gates repaints |
| `gDebugMenuVramSnap` | (EWRAM free pool) | Private BG0/BG1 maps + black tile snapshot |

Unknown pool-backed addresses use `gUnk_XXXXXXXX` in the `*_pool.inc` files. Promote to a named symbol in `ram_map_*.s` (+ `KNOWN_*` in the scanner) when the role is identified.

## Practical rules

1. Put shared macros in `asm/ram_map.s`.
2. Add named symbols to the matching region file; refresh pool incs with the scanner.
3. Prefer cursor allocators only for adjacent custom blocks.
4. Stay below the user stack in IWRAM; do not use the EEPROM cart window as if it were Flash save without verifying hardware behavior.
