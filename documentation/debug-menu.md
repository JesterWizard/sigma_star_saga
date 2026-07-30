# Overworld Debug Menu

---

## Index

- [Introduction](#introduction)
- [Plan](#plan)
- [Controls](#controls)
- [Display model](#display-model)
- [Engine cooperation](#engine-cooperation)
- [Code Locations](#code-locations)
- [TODO](#todo)
- [Limitations & Bugs](#limitations--bugs)

## Introduction

Vanilla Sigma Star Saga only saves from the status / END-SAVE UI. For hack development that is awkward: you have to leave the field, open SELECT status, and hope the slot path still works.

This feature adds a **mode-preserving** overworld overlay:

1. **START** on a walking field mode opens a black screen with a scrollable option list.
2. **Save game** — **A** saves the current slot anywhere (END-SAVE style EEPROM commit).
3. **Warp to scene...** — pick a NAV location (Starbase / planet / palace / …) and **A** queues that mode.
4. **Boss fight...** — pick a boss and **A** drops you straight into that fight.
5. **START / B** closes and restores the field (HUD, cameras, VRAM tiles).

It deliberately **never** calls `StatusToggle`, `StatusPanel`, `SetMode(0x168)`, or `LeaveStatusRestore`. `gMode` stays on the overworld value while the menu is open; world sim is paused by LynJumps on the overworld frame, not by changing modes. Both warps and bosses close the overlay and then `QueueModeFade`; a boss additionally sets `gStageCase` first so the destination mode knows which arena to load.

Toggle: `configs/runtime.c` → `.debug_menu`.

## Plan

| Layer | Behavior |
|-------|----------|
| Gate | `.debug_menu` + overworld field mode (mode JT entry = `OverworldMainFrame` @ `0x0800D610`) + status panel closed |
| Open | START edge → snapshot display / HUD / cameras / charbase-2 → load debug font → black overlay |
| Idle | World frame body skipped; menu owns BG0 text + BG1 black fill |
| Root | `Save game` / `Warp to scene...` / `Boss fight...` with UP/DOWN cursor |
| Save | A → `Saving…` → `SetSaveSlot` → `FlushSaveMeta` → `WriteSave` → `Saved!` (~90 frames) |
| Warp | A opens NAV location list; A again → close overlay → `QueueModeFade(modeId)` |
| Boss | A opens the boss list; A again → set `gStageCase` → close → `QueueModeFade(132)` |
| Close | START (any page) / B (root) → restore mirrors + cameras + font tiles → dirty all soft maps |
| Toggle off | If `.debug_menu` is FALSE at runtime, any open menu force-closes and LynJumps leave vanilla frame code |

```mermaid
flowchart TD
  field["Overworld field"]
  open["START → DebugMenu_Begin"]
  root["Root: Save / Warp / Boss"]
  warp["Warp location list"]
  boss["Boss list"]
  save["A → DebugMenu_DoSave"]
  goWarp["A → Close + QueueModeFade"]
  goBoss["A → gStageCase + Close + QueueModeFade(132)"]
  close["START/B → DebugMenu_Close"]
  field --> open --> root
  root --> save --> root
  root --> warp
  root --> boss
  warp --> goWarp --> field
  boss --> goBoss --> fight["Boss arena (gMode 132)"]
  root --> close --> field
  warp --> close --> field
  boss --> close --> field
```

## Controls

| Input | When | Effect |
|-------|------|--------|
| START | Field, menu closed | Open menu |
| UP / DOWN | Menu idle | Move cursor (list scrolls past 18 rows) |
| A | Root → Save | Save current slot; show `Saving…` then `Saved!` |
| A | Root → Warp / Boss | Enter that submenu |
| A | Warp list | Close menu and `QueueModeFade` to that location's mode ID |
| A | Boss list | Set `gStageCase`, close menu and `QueueModeFade` to the arena mode |
| B | Warp / Boss list | Back to root |
| B / START | Root idle | Close and restore overworld |
| START | Submenu | Close and restore overworld |
| A / B / START | During `Saved!` timer | Dismiss status text early |

SELECT / status is untouched. Opening status while the menu is somehow still marked open force-closes the overlay.

### Warp destinations

Names and mode IDs come from the vanilla NAV table @ `0x0824F348` (24 entries: `u32 modeId` + `name[0x20]`). Examples: `EARTH`→9, `STARBASE1`→18, `FOREST`→4, `FIRE`→5, `FORGOTTEN`→17, `PALACE`→16, `LAUNCH EARTH`→24.

Warps use `QueueModeFade` (`0x0800D734`) — same path as StatusToggle: transition latch + fade-to-black + pending mode @ `0x03000D6C`. `FadeStep` applies `gMode` when the fade completes; map prep then runs from `OverworldMainFrame`.

### Boss fights

A boss fight is a **stage**, not a mode. The `B_*` / `MB_*` ids in the stage-label table (`0x080ED50C`) are `SetMode` GFX ids — feeding one to a mode change loads tiles but never leaves the field, which is why earlier `SetMode`-based attempts only produced VRAM garbage.

The arena the engine actually enters is selected by two values:

| Value | Meaning |
| --- | --- |
| `gMode` 132 | init = `EnterStageArena` @ `0x08028A44`, frame = stage FSM @ `0x08028B40` |
| `gStageCase` @ `0x03007738` | index into the arena jump table @ `0x0802972C` (0…268) |

`EnterStageArena` @ `0x080296D4` reads `gStageCase`, `SetMode`s that arena, loads its map and spawns both the player and the arena's actors. So confirming a boss is just `gStageCase = case` then the ordinary warp fade:

```c
gStageCase = entry->stageCase;
DebugMenu_Close();
QueueModeFade(132, QUEUE_FADE_SPEED);
```

Cases 135–174 cover the four boss arenas in blocks of ten — one case per approach segment, plus the single case that spawns only the boss. Those are the four rows in the list:

| Row | `gStageCase` | Arena label |
| --- | --- | --- |
| `B_DRILL` | 142 | 246 |
| `B_CONCENTRATOR` | 147 | 247 |
| `B_BLUNE` | 161 | 248 |
| `B_LAVAWORM` | 165 | 249 |

Each was checked end-to-end in `tools/mgba_boss_probe.c`: replay START / DOWN / A, then confirm `gMode` stays 132, the arena `SetMode` id matches and the live enemy count @ `0x03007080` is exactly 1 (the boss) for 20+ seconds.

## Display model

While open:

| Layer | Setup | Role |
|-------|--------|------|
| BG0 | CB2 / SB30, pal bank 15 | Option list / `Saving…` / `Saved!` |
| BG1 | CB2 / SB31, blank tile 0 | Solid black fill |
| BG2–BG3 / OBJ | Off via `gDisplayCtrlMirror` | Hide world + HUD |

Text is written **directly** into VRAM screenbase 30. Soft-text (`ClearSoftTextMap` / `DrawDebugText`) is avoided: those set `gSoftTextDirty`, and VBlank then DMA-reloads soft maps every frame and fights the overlay.

Repaints only run when `gDebugMenuTextState` changes (cursor moves force `DBG_TEXT_NONE`), and they wait on VBlank first. Clearing the whole 32×32 map mid-frame was what made the text tear / flicker in and out.

## Engine cooperation

Two engine paths still run every frame from the main-loop epilogue @ `0x0800BB00`, **outside** both LynJump sites. Pausing the overworld frame body does not stop them:

| Path | Address / symbol | What the menu does |
|------|------------------|--------------------|
| `HudSync(1)` | `0x08010E58`, called from `0x0800BB98` | Rebuilds HUD tilemap unless `gHudEnabled == 0` |
| Status DISPCNT mirror | `gDisplayCtrlMirror` @ `0x03007194` | Re-applied to `REG_DISPCNT` every frame; menu drives this mirror, not hardware alone |

On close, restore:

1. `gDisplayCtrlMirror`, `gHudEnabled`, soft DISPCNT, hardware `BGxCNT`, blend/windows, IE/DISPSTAT.
2. Full `gCameras` array (font load re-points layer 3; overlay zeroes active flags / scroll).
3. Charbase-2 tile snapshot (`gDebugMenuVramSnap`, `0x4000` bytes).
4. `gSoftTextDirty = 0x1F` so VBlank repaints every soft map the overlay wrote over.

`gSoftBg2Pa` / `gSoftBg2Pb` (`0x03001E7C` / `0x03001F6C`) are **affine batch** mirrors, not BG0/BG1 soft CNT. Do not treat them as BGxCNT shadows.

## Code Locations

| Feature | Location | Description |
|---------|----------|-------------|
| Runtime toggle | `.debug_menu` in `configs/runtime.c` | Enables LynJumps + menu logic |
| Menu body | `DebugMenu_*` in `src_custom/debug_menu_hooks.c` | Open / present / save / warp / boss / close |
| Public API | `DebugMenu_IsBlocking` / `DebugMenu_OnOverworldFrame` in `include/debug_menu.h` | Gate + per-frame entry |
| Mode thunks | `ChangeMode` / `QueueModeFade` / `EnterModeScene` in `include/status.h` | Warp + special scene entry |
| Boss table | `sDebugBosses` in `src_custom/debug_menu_hooks.c` | Name + `gStageCase` per boss arena |
| Arena selector | `gStageCase` in `asm/ram_map_iwram.s` | Index into the arena JT @ `0x0802972C` |
| Frame LynJumps | `OverworldMainFrame__Replacement` / `OverworldFrameTail__Replacement` | Skip world body / cameras while open; jump to epilogue `0x0800D6CF` when blocking |
| Player gate | `DebugMenu_IsBlocking` call in `src_custom/flight_skip_hooks.c` | Freeze player update while menu open |
| LynJump apply | `apply_debug_menu` in `tools/apply_lynjump.py` | Hooks @ `0xD610` (main) and `0xD66A` (tail) |
| Save path | `SetSaveSlot` / `FlushSaveMeta` / `WriteSave` in `src/save.c` | END-SAVE style; not player-FSM `SaveCommitPrep` |
| Warp table | ROM `0x0824F348` | NAV `modeId` + name (24 entries, stride `0x24`) |
| EWRAM state | `gDebugMenu*` in `asm/ram_map_ewram.s` | Active, screen, scroll, text state, snapshots |
| IWRAM mirrors | `gDisplayCtrlMirror` / `gHudEnabled` in `asm/ram_map_iwram.s` | Status-module DISPCNT shadow + HudSync enable |
| Probe harness | `tools/mgba_debug_menu_probe.c` | Headless open / save / close + framebuffer dumps |
| Boss probe | `tools/mgba_boss_probe.c` | Replays the boss rows; also sweeps `gStageCase` ranges |

## TODO

- [ ] More menu entries (flags, item grants) behind the same overlay
- [ ] Pin full BG palette bank 15 so text color does not inherit field leftovers
- [ ] Drop temporary `DEBUG_MENU_LOG` / No$ prints once the overlay is considered stable
- [ ] Optional non-blocking save (today `WriteSave` freezes the CPU for ~20 frames on EEPROM)
- [ ] Friendlier warp / boss labels (Title Case, named bosses) and optional raw ID pickers
- [ ] Reach the flight-battle bosses (`B_MEATHEAD`, `MB_*`, …) — they need a battle record, not a stage case
- [ ] Optional rows for the approach segments of each boss stage (the other nine cases per block)
- [x] Fade-out before warp via `QueueModeFade` (StatusToggle path)
- [x] Boss entry via `gStageCase` + `QueueModeFade(132)`

## Limitations & Bugs

- Opens only when the current `gMode`'s main-loop JT slot points at `OverworldMainFrame` (`0x0800D610`). Flight, cutscenes, and status mode are out of scope.
- Save uses the **current** `gSaveSlot`. There is no in-menu slot picker yet.
- `WriteSave` polls EEPROM ready bits in ROM and blocks for roughly a frame per word; the menu shows `Saving…` so the freeze is intentional feedback, not a hang.
- Warp uses `QueueModeFade` (fade then pending `gMode`); spawn position / facing come from whatever the destination mode's prep uses (not a full door-record teleport).
- The boss list only holds the four **stage** bosses (`B_DRILL`, `B_CONCENTRATOR`, `B_BLUNE`, `B_LAVAWORM`). The remaining `B_*` / `MB_*` labels are backdrops loaded during a flight battle: their gModes (24–30, 151) do `SetMode` the arena, but they spawn no enemies, so the stage-clear check fires immediately and chains to the next stage. Reaching them needs a battle record, not a stage case.
- Entering a boss stage skips whatever story flags the arena normally expects, so clearing it may advance the stage chain from wherever it thinks you are.
- With `.skip_flight_battle` on, **SELECT+L** still force-clears an in-progress flight stage.
- Text color can look cyan/orange/etc. depending on leftover entries in palette bank 15; only indices 0 and 1 are forced black / white.
- Charbase 2 is shared with the HUD and some world layers; the overlay snapshots and restores it, but a failed close path will leave garbage glyphs in the field.
- Soft BG0/BG1 CNT mirrors do not exist; snapshotting the old mislabeled affine symbols will not restore layer setup.
- Headless mGBA may need `GBASavedataForceType(..., SAVEDATA_EEPROM512)` — autodetect can leave type unresolved and the ready poll never completes.

Report save/restore/warp/boss glitches with the field `gMode`, destination / battle ID, whether `.debug_menu` was on, and a screenshot of the open and closed frames.
