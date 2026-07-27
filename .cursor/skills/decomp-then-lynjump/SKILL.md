---
name: decomp-then-lynjump
description: >-
  Required workflow for any new gameplay/UI/hack feature in this repo: first
  decompile the relevant vanilla functions into src/, peel them into the ROM
  build, then implement behavior via LynJump replacements in src_custom/. Use
  whenever the user asks for new functionality, features, hacks, toggles,
  overlays, HUD, combat changes, or anything that touches vanilla game code.
---

# Decomp Then LynJump

**Always use this skill when the user asks for new functionality.** Do not
implement features by only reverse-engineering call sites and writing append
hooks against opaque absolute addresses. That caused soft-OAM / VRAM corruption
and frame-rate collapse when HP bars were added without decomping `DrawActors`.

Also read and follow:
- `.cursor/skills/prefer-c-over-asm/SKILL.md` — implement in C; avoid new ASM
- `.cursor/skills/lyn-jump-hook-placement/SKILL.md` — where hook bodies go
- `.cursor/skills/ram-map/SKILL.md` — when new RAM is needed

## Mandatory Order

```
1. Identify vanilla entrypoints (ROM addr + Thumb signature)
2. Decompile those functions into src/*.c (+ include/*.h)
3. Peel them into the matching ROM image (asm peels / ld_script / Makefile)
4. Add runtime toggle if the feature is optional
5. Implement Name__Replacement in src_custom/*_hooks.c
6. Wire LynJump / apply_lynjump.py
7. make and verify (no graphics corruption, no FPS collapse)
```

Skip steps only when the needed functions are **already** decompiled in `src/`
and linked at their vanilla addresses. Still implement the hack in `src_custom/`.

## Step 1 — Identify Entrypoints

- Find the smallest set of vanilla functions the feature must observe or change.
- Prefer whole-function boundaries over mid-function patches.
- Record: symbol name (invent if unknown), ROM offset, size, callers, callees.
- Disassemble from `baserom.gba`; do not invent struct layouts without evidence.

## Step 2 — Decompile Into `src/`

- Write readable C under `src/<area>.c` (and `include/<area>.h` as needed).
- **C, not ASM** — do not leave or rewrite the target as a hand-maintained `.s`
  body; peels exist so the function lives in C.
- Match pret/pokeemerald style used by existing peels (see `src/level_up.c`).
- Name globals via the ram-map skill; do not hard-code `0x03…` / `0x02…` in new C.
- Prefer accurate control flow and types over byte-identical matching on the first
  pass, but keep the function at the correct ROM span when peeling.
- **Do not** put custom hack logic in `src/` — vanilla behavior only.

## Step 3 — Peel Into The Build

Existing peel pattern (`level_up`):

| Piece | Role |
|-------|------|
| `asm/rom.s` (and `rom_after_*.s`) | Shrink `.incbin` so the decompiled span is not double-included |
| `src/<file>.c` | Decompiled body |
| `Makefile` `C_SRCS` | Compile the new `src/` file |
| `ld_script.ld` `.rom` | Place `src/<file>.o(.text)` (and data) at the peeled address gap |

After peel, `make` must still produce a coherent ROM. Custom append / LynJump
already breaks `make compare`; peels should still line up with baserom ranges.

## Step 4 — Runtime Toggle

If the feature is optional, add a field to `RuntimeConfig` in
`include/runtime.h` and `configs/runtime.c`, and teach
`tools/apply_lynjump.py` (`BOOL_RE`, `load_runtime_flags`, enable/restore).

## Step 5 — LynJump Replacement In `src_custom/`

- Body in `src_custom/<feature>_hooks.c` with `APPEND_TEXT`.
- Name: `VanillaName__Replacement`.
- Call decompiled / continued vanilla via `VanillaName__Continue` trampoline
  when the veneer stole the prologue (`asm/<feature>_trampoline.s`).
- Prefer calling the **now-named** vanilla function (or `__Continue`) over raw
  `((Func)0x08……|1)()` once it lives in `src/`.
- Gate with `gRuntimeConfig.<flag>` when toggled.
- Wire: `src_custom/LynJump.event` and/or `tools/apply_lynjump.py`.
- Add sources to `Makefile` `CUSTOM_C_SRCS` / `ASM_SRCS`.

Follow **lyn-jump-hook-placement** for veneer patterns, restore-on-disable, and
section attributes.

## Hard Rules (from the HP-bar failure)

- **No speculative graphics.** Do not write soft OAM, OBJ VRAM, or palettes until
  the draw/flush path is decompiled and understood (`DrawActors`, OAM submit,
  tile ownership).
- **No heavy per-frame work** without measuring: full VRAM rebuilds or pool-wide
  scans every frame need a clear budget.
- **No inventing actor/HUD layouts** beyond documented offsets; extend the decomp
  when a field is unknown.
- **Do not** ship a feature that only append-hooks unknown code. Decomp first.

## Done Checklist

- [ ] Needed vanilla functions exist as C in `src/` (or were already there)
- [ ] Peels / `ld_script.ld` / `Makefile` updated if newly decompiled
- [ ] Custom logic only in `src_custom/` via LynJump
- [ ] Runtime toggle + apply_lynjump restore path (if optional)
- [ ] RAM map updated for any new globals
- [ ] `make` succeeds; in-game: no corruption, acceptable frame rate
