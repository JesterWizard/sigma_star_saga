---
name: prefer-c-over-asm
description: >-
  Prefer C over hand-written ASM for all coding work in this repo. Use whenever
  writing, editing, decompiling, hooking, peeling, patching, or adding game /
  hack / UI / build logic — implement in src/ or src_custom/ C first; avoid new
  .s bodies unless fixed-address glue is unavoidable.
---

# Prefer C Over ASM

**Any coding work should produce C. ASM should be avoided as much as possible.**

Write and change logic in `src/*.c` (vanilla peels) or `src_custom/*_hooks.c` /
`src_custom/*_data.c` (custom). Do not reach for new `.s` files or inline asm
as the default implementation.

## Default

| Work | Put it in |
|------|-----------|
| Decompiled vanilla | `src/<area>.c` + `include/<area>.h` |
| Hack / LynJump body | `src_custom/<feature>_hooks.c` (`APPEND_TEXT`) |
| Custom tables / data | `src_custom/<feature>_data.c` or JSON pipelines |
| New globals | ram-map symbols + C externs — not ad-hoc asm defs in feature files |

## ASM only when unavoidable

Keep hand-written ASM to the smallest fixed-site glue:

- LynJump / Thumb veneer bytes applied by `tools/apply_lynjump.py` (not a new logic `.s`)
- `Name__Continue` trampoline at a stolen prologue (`asm/<feature>_trampoline.s`)
- `asm/rom*.s` / `ld_script.ld` peels that carve a span for already-written C
- Tiny restore-gated in-place patches that cannot be expressed as a C replacement

If ASM is required, the behavior still lives in C; the `.s` only bridges to it.

## Do not

- Reimplement gameplay, HUD, combat, or UI in `.s`
- Add mid-function asm patches when a whole-function C peel + LynJump works
- Expand existing asm peels with custom logic (belongs in `src_custom/`)
- Prefer asm "because the original was asm" — decompile to C instead

## Related

- New features: `.cursor/skills/decomp-then-lynjump/SKILL.md`
- Hook placement: `.cursor/skills/lyn-jump-hook-placement/SKILL.md`
- RAM symbols: `.cursor/skills/ram-map/SKILL.md`
