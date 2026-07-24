---
name: lyn-jump-hook-placement
description: >-
  Use when adding or moving custom hack logic in this repo; place the body in
  `src_custom/*_hooks.c` (APPEND_TEXT `__Replacement`) and wire it through
  LynJump / `tools/apply_lynjump.py` instead of editing vanilla `src/` or
  baserom peels for gameplay changes. For brand-new features, follow
  decomp-then-lynjump first (decomp into src/, then hook).
---

# Lyn Jump Hook Placement

Use this skill when creating or moving custom functionality (hacks, feature
replacements, runtime-gated behavior).

**New features:** read `.cursor/skills/decomp-then-lynjump/SKILL.md` first.
Decompile the touched vanilla functions into `src/`, then put the hack body
here under `src_custom/`. Do not LynJump opaque ROM without a decomp.

## Core Rule

- Do not put new hack bodies in vanilla `src/*.c` or by rewriting large peels in
  `asm/rom*.s` for custom behavior.
- Put each feature’s logic in hook-side files under `src_custom/`:
  - `src_custom/<feature>_hooks.c` — `APPEND_TEXT` replacements / helpers
  - `src_custom/<feature>_data.c` — `APPEND_RODATA` / `APPEND_DATA` tables when needed
  - optional `include/<feature>.h` for shared constants and prototypes
- Keep vanilla / peel asm as the call site only when an in-place stub or
  trampoline must live at a fixed ROM address.
- Gate behavior with `configs/runtime.c` + `include/runtime.h` whenever the
  feature should be toggleable.

## Where To Look First

- Hook body + naming:
  - `src_custom/flight_skip_hooks.c` — per-frame / EXP veneers
  - `src_custom/suction_hooks.c` — Impact / gem-update replacements
  - `APPEND_TEXT` / `APPEND_RODATA` / `APPEND_DATA` in `include/runtime.h`
- Wiring / patches:
  - `src_custom/LynJump.event` — `ORG` + `POIN` / `WORD` stubs applied by
    `tools/apply_lynjump.py`
  - `tools/apply_lynjump.py` — runtime-gated veneers, pointer redirects, and
    restores from `baserom.gba` when a flag is off
- Toggles:
  - `configs/runtime.c`
  - `include/runtime.h` (`RuntimeConfig`, `LYN_REPLACE_CHECK`)
- Mid-function continue after a stolen prologue:
  - `asm/suction_trampoline.s` (`Name__Continue`)
- Build membership:
  - `Makefile` `CUSTOM_C_SRCS` / `ASM_SRCS`
- Data-only custom content (not LynJump):
  - `src_custom/dialogue/` + `tools/compile_dialogue.py`
  - `src_custom/data_structures/` + `tools/compile_data_structures.py`
- RAM symbols used by hooks:
  - use the **ram-map** skill (`asm/ram_map_*.s`, `include/ram_map.h`)

Search the matching `*_hooks.c` and `apply_lynjump.py` owner first, then the
feature header / trampoline. Avoid broad repo-wide searches unless those do not
mention the entrypoint.

## Hook Patterns (pick one)

| Pattern | When | Example |
|---------|------|---------|
| Long-jump veneer | Replace a whole Thumb function (or first 8–16 bytes) with `Name__Replacement` | `UpdateShooterFrame__Replacement` @ `0x14E70`; gem updates via `apply_veneer` |
| Icon / special veneer | Prolog must preserve extra args (e.g. out-ptr in `r3`) | `GetGunDataIconFrame__Replacement` + `apply_icon_veneer` |
| Replacement + `__Continue` | Steal prologue, run custom code, resume vanilla | `ExpGemUpdate__Replacement` → `ExpGemUpdate__Continue` |
| In-place asm | Tiny fixed-site patch that stays in the 8MB image | `asm/flight_skip.s` @ `0x1749C`, gated by restore-from-baserom |
| Pointer / table redirect | Relocate or extend a ROM table in append space | Suction `gImpactJumpTable` / `gImpactDescTable` |
| Dialogue / JSON tables | Content banks, not function hooks | `custom_dialogue`, `custom_enemy_exp` |

Default veneer (Thumb): `ldr r3, [pc, #0]; bx r3; .word hook|1` — see
`SHOOTER_FRAME_VENEER_HEAD` in `apply_lynjump.py`.

## Fast Path

1. Add or reuse a toggle in `configs/runtime.c` / `include/runtime.h`.
2. Implement `Name__Replacement` (and helpers) in `src_custom/<feature>_hooks.c`
   with `APPEND_TEXT`. Call vanilla via absolute Thumb addresses (`|1`) or a
   `Name__Continue` trampoline when the prologue was overwritten.
3. Add the `.c` (and any trampoline `.s` / `_data.c`) to `Makefile` source lists.
4. Wire the entry:
   - Simple symbol poke → `src_custom/LynJump.event` (`ORG $offset` + `POIN Name__Replacement`)
   - Runtime-gated / multi-site / restore-on-disable → extend `tools/apply_lynjump.py`
     (parse the new flag in `BOOL_RE` / `load_runtime_flags`, apply or restore
     from `baserom.gba`, resolve symbols via `arm-none-eabi-nm`)
5. `make` — linker places append code past 8MB; `apply_lynjump.py` patches the ROM.
6. Do **not** expect `make compare` to stay green once append / LynJump is linked.

## Workflow

1. Identify the vanilla entrypoint (ROM offset + Thumb signature).
2. Decide pattern: veneer, continue-trampoline, in-place asm, or table redirect.
3. Place the body in `src_custom/<feature>_hooks.c` (one feature / related
   cluster per file; reuse an existing file if the same subsystem already owns
   the hook).
4. If new RAM is needed, update the RAM map first (ram-map skill).
5. Wire LynJump.event and/or `apply_lynjump.py`; keep disable paths restoring
   `baserom.gba` bytes for every owned site.
6. Keep any edit inside the 8MB image as small as possible (veneer bytes,
   cmp immediates, pointer words) — not full reimplementations.

## Naming & Sections

- Replacements: `VanillaName__Replacement`
- Continuations: `VanillaName__Continue` (asm trampoline)
- Text / const / mutable append: `APPEND_TEXT` / `APPEND_RODATA` / `APPEND_DATA`
- Optional compile-time existence check: `LYN_REPLACE_CHECK(symbol)`

## Notes

- Prefer one feature cluster per `*_hooks.c`; share helpers inside that file or
  a small `include/<feature>.h`.
- Prefer named RAM map symbols over hard-coded `0x02…` / `0x03…` addresses.
- If an effect already has a hook-side home, extend that file instead of adding
  vanilla logic.
- Dialogue and JSON data tables are append content with pointer redirects — not
  card-style per-ID hook files; follow `documentation/dialogue/` and
  `src_custom/data_structures/README.md`.
- Overlapping patch owners fail in `apply_lynjump.py` (`checked_write`) — claim
  distinct byte ranges per feature.
- Matching / decomp work stays in `src/` and `asm/` peels; custom gameplay
  always goes through this hook path.
