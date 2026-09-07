# Architecture

Sigma Star Saga is a commercial GBA ROM (`baserom.gba`) being modded/patched
via a hybrid decompile-and-append strategy, not a full decompile. Read this
before touching build output, hook wiring, or the runtime toggle system —
it'll save you re-deriving the pipeline from scratch.

## The mental model

1. `baserom.gba` is the unmodified retail ROM. It is never edited directly.
2. `src/*.c` holds **decompiled** vanilla functions — real C recreations of
   specific baserom addresses, compiled and appended into the ROM to
   *replace* the original machine code at that address.
3. `src_custom/*.c` holds **new** functionality that has no vanilla
   equivalent (debug menu, runtime cheat toggles, custom gun data, event
   runner, GAX audio extensions). These call into vanilla ROM addresses as
   opaque function pointers/hooks — there's no compiler-checked struct
   backing them, just raw offsets (see `include/ram_map.h` and
   `documentation/ram-map.md`).
4. `tools/apply_lynjump.py` is the final build step: it patches compiled
   veneers/hooks into the ELF/ROM at specific byte offsets, guarded by the
   `RuntimeConfig` flags in `configs/runtime.c`. This is also where
   generated data tables (dialogue, gun data, event scripts) get baked in.
5. Everything is gated by **build-time** flags in `configs/runtime.c`
   (`gRuntimeConfigRom`) — a cheat/feature that's `FALSE` there may have its
   entire hook *not installed at all* in the ROM, regardless of what
   in-game UI (like the debug menu) claims to control it. See "Two kinds of
   'off'" below — this exact confusion cost real debugging time once.

## Directory map

| Path | What's in it |
|---|---|
| `src/*.c` | Decompiled vanilla functions, one file per subsystem (actor, damage, dialogue, cutscenes, overworld, save, GAX audio). Comment header always states the baserom address + span it replaces. |
| `src_custom/*.c` | New hooks/features with no vanilla equivalent — debug menu, runtime toggles, custom gun data, event runner, random battle gate, overworld EXP, GAX extensions. |
| `include/*.h` | Headers for both of the above, plus `ram_map.h` (EWRAM/IWRAM symbol table — the canonical address registry, do not hardcode `0x03…`/`0x02…` addresses elsewhere), `runtime.h` (the `RuntimeConfig` struct + `LYN_REPLACE_CHECK`/`APPEND_*` macros), `status.h` (vanilla mode-transition function pointers: `SetMode`, `ChangeMode`, `QueueModeFade`, …). |
| `asm/*.s` | Hand-written trampolines (`*_trampoline.s` — the `__Continue` stubs that reissue a hook's stolen prologue bytes and resume mid-function), plus `ram_map*.s` (the actual EWRAM/IWRAM address registry `ram_map.h` documents) and `rom*.s` (raw ROM segment definitions). |
| `src_custom/LynJump.event` | Declarative full-function replacements in `ygodm8` LynJump format (Thumb `bx pc` → ARM `ldr`/`bx` stub) — used for the handful of hooks (`PlayerHitUpdate`, `PlayerStateMachine`, `PlayerShipUpdate`) that need a 16-byte prologue steal rather than the simpler 8-byte veneer `apply_lynjump.py` installs elsewhere. |
| `tools/apply_lynjump.py` | The real install script. Reads `RuntimeConfig` flags, decides which veneers/LynJump hooks to install vs. restore-to-vanilla, and bakes in generated data tables. **This is where a feature can be silently absent from the ROM even though the C code compiled fine** — always check the matching `apply_*` function's enable condition when a hook "doesn't seem to run." |
| `tools/compile_*.py` | Data-table generators (dialogue banks, gun/cannon/bullet data from JSON, event scripts) consumed by `apply_lynjump.py`. |
| `tools/extract_*.py`, `tools/scan_ram_literals.py` | One-off reverse-engineering helpers, not part of the normal build. |
| `configs/runtime.c` | The single source of truth for which cheats/features are compiled into the ROM at all (`gRuntimeConfigRom`). Edit this, rebuild — see "Two kinds of 'off'" below for why toggling this isn't the same as toggling a debug-menu row. |
| `constants/`, `data/`, `events/`, `graphics/`, `sound/` | Generated/extracted asset and data-table inputs to the `compile_*.py` scripts. |
| `documentation/*.md` | Deep-dive docs per subsystem — `ram-map.md` (address registry mechanics), `debug-menu.md` (the in-game START overlay, its known limitations, and its "Limitations & Bugs" section which is the best model of how thorough this project's bug write-ups should be), `gax-audio.md`, `dialogue/`, `events/`. |
| `.claude/skills/*` | Debugging methodology, written up as reusable skills after being learned the hard way. Read the skill descriptions before starting a hook/RAM-address/live-verification task — see "Debugging skills" below. |

## Two kinds of "off" — the confusion that costs the most time

A feature in this codebase can fail to work for two structurally different
reasons, and conflating them wastes debugging effort in opposite
directions:

1. **The hook was never installed in the ROM at all.** `apply_lynjump.py`'s
   `apply_*` functions gate veneer installation on `RuntimeConfig` flags
   (sometimes a feature's *own* flag, sometimes a shared flag from an
   unrelated-looking feature — e.g. the "Enemy HP bars" LynJump hook is
   installed whenever `enemy_hp_bars` **or** `debug_menu` is `TRUE`, because
   the debug menu's live toggle needs the hook present to have anything to
   toggle). If the hook isn't installed, no runtime flag anywhere can make
   the feature work — the underlying machine code literally isn't there.
   **Check**: does the build log line for that feature print, and does the
   symbol's ROM address actually redirect to your replacement (`xxd` the
   patched offset, decode the veneer)?
2. **The hook is installed and running, but reads the wrong condition or
   the wrong address.** This is a normal logic bug, fixable without
   touching `apply_lynjump.py`.

A debug-menu toggle can *look* like case 2 (it's clearly present in the
UI, flips visibly) while actually being case 1 underneath for a specific
code path that never got wired up — see `documentation/debug-menu.md`'s Max
Health entry for a fully-worked example where the HP-pinning hook was
correct and installed, but a *separate* death-decision code path never
went through it at all.

## Confirmed-live address log

Several RAM addresses in `include/ram_map.h` have a plausible-sounding name
(`gPlayerPtr` = "the player pointer") that turned out to be **wrong or
only-sometimes-true** once actually checked live in mGBA, and re-deriving
that was expensive enough to be worth never doing twice. The log lives in
`documentation/ram-map.md`'s **"Confirmed live"** section(s), not here —
check there before trusting a symbol's name for anything stage-dependent
(the player/ship pointer, encounter selectors, anything read differently
across `gMode` values).

**When you spend real probe time confirming (or debunking) what an address
actually holds in a specific stage/mode, add a "Confirmed live" entry to
`ram-map.md`** with: the address, what it turned out to be, which
stage/mode you checked it in, and how you checked (position-tracking,
value-tracking, a specific probe technique) — not just the conclusion. The
next investigation needs the method as much as the answer, since the same
address can behave differently in a stage nobody's checked yet.

## Debugging skills (read before, not after)

- **`.claude/skills/gba-ram-address-audit`** — before trusting any
  `gUnk_*`/undocumented RAM address, classify it (boolean flag vs.
  per-context selector vs. shared scratch) from real write-site
  disassembly. Skipping this is how a plausible-sounding "clear this to
  reset" hook corrupts unrelated state.
- **`.claude/skills/rom-hook-regression-check`** — three specific failure
  patterns that recur in this repo's hooks (over-broad jump-table
  membership tests, backwards flag polarity, restore helpers touching
  shared state) plus the A/B-with-negative-control verification method.
- **`.claude/skills/mgba-headless-probe`** — how to build and run a
  headless mGBA probe against this ROM, including the two-different-mGBA-
  builds trap (savestates from the user's real desktop app need a newer
  library than the prebuilt one) and proven boot sequences to real
  gameplay from both a fresh boot and a real save file.
- **`.claude/skills/gba-causal-trace-hunt`** — once a fix pins/vetoes one
  field but the reported symptom persists, how to actually find the real
  causal write instead of iterating on plausible-looking variants of the
  same wrong fix.
- **`.claude/skills/verification-scope-honesty`** — how to report status
  when the root cause is fixed and A/B-tested but the literal end-to-end
  gameplay symptom couldn't be watched to stop (RNG-gated content,
  story-locked stages, a scripted probe that can't play skillfully). Says
  which claims are earned at which verification tier — don't round up.

## Build

`make` from the repo root. See `Makefile` for the full source list; the
short version is: decompiled `src/*.c` and custom `src_custom/*.c` compile
normally, link against `baserom.gba`-derived object files
(`asm/rom*.s`), then `tools/apply_lynjump.py` runs as a post-link step to
patch veneers and data tables into the final `.gba`. **The build log from
`apply_lynjump.py` is the ground truth for what actually got installed** —
read it, don't assume from the C source alone.
