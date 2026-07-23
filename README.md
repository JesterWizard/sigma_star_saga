# Sigma Star Saga

Decompilation scaffold for *Sigma Star Saga* (GBA), structured after [pret/pokeemerald](https://github.com/pret/pokeemerald). Custom hacks follow the [ygodm8](https://github.com/JesterWizard/ygodm8) pattern: append ROM past the retail image, LynJump hooks, and `configs/runtime.c` toggles.

## Status

Early scaffold: the ROM is rebuilt by `.incbin`-ing `baserom.gba` so `make compare` can pass before any code is matched. Peel ranges out of `asm/rom.s` into real asm/C as work progresses. Custom code linked past 8MB breaks `make compare`.

### Flight battle skip (hack)

Toggle `.skip_flight_battle` in [`configs/runtime.c`](configs/runtime.c). When `TRUE`, hold **Select + L** during a 2D flight/shmup stage to force a stage clear. Implemented in-place at `0x0801749C` (`asm/flight_skip.s`).

### Always run (hack)

Toggle `.always_run` in [`configs/runtime.c`](configs/runtime.c). When `TRUE`, overworld movement uses run speed without holding **B**.

### Always max health / bombs (hack)

Toggle `.always_max_health` and `.always_max_bombs` in [`configs/runtime.c`](configs/runtime.c). During 2D flight stages, hurt state is cancelled each frame (the HUD enemy counter at `gEnemyRemaining` is left alone), and smart bombs stay at the maximum (7).

### Gun Data / items (hack)

Toggle the Gun Data and item unlocks in [`configs/runtime.c`](configs/runtime.c):

- `.all_cannon_data` / `.all_bullet_data` / `.all_impact_data` — unlock every piece of that Gun Data type
- `.all_key_items` — DATA CARD, GENOME, VIRUS SAMPLE, and related key bits
- `.all_overworld_items` — flares, fossils, hints, and related overworld bits

## Quick start

See [INSTALL.md](INSTALL.md).

```bash
# after placing baserom.gba
make
# matching build (no append / LynJump — not the default once custom is linked)
make compare
```

## Layout

| Path | Role |
|------|------|
| `asm/` | Hand-written / disassembled ARM/Thumb + baserom peels |
| `asm/ram_map*.s` | IWRAM / EWRAM / save address registry ([ygodm8](https://github.com/JesterWizard/ygodm8/blob/master/asm/ram_map.s)-style) |
| `src/` | Decompiled vanilla C |
| `src_custom/` | Hack hooks (`*_hooks.c`) + `LynJump.event` |
| `configs/runtime.c` | Boolean / value toggles for hacks |
| `include/`, `include/gba/` | Headers (GBA register/types from pret style) |
| `documentation/ram-map.md` | How to allocate and extend the RAM map |
| `data/`, `graphics/`, `sound/` | Extracted assets (empty for now) |
| `ld_script.ld` | Memory map; append region starts after the 8MB baserom |
| `tools/apply_lynjump.py` | Post-link absolute jump stubs into the ROM |
| `rom.sha1` | Matching checksum for `make compare` |

## Matching workflow

1. Disassemble / analyze `baserom.gba` (Ghidra, etc.).
2. Replace a range in `asm/rom.s` with a real object in `asm/` or `src/`.
3. Update `ld_script.ld` and the Makefile source lists.
4. `make compare` — keep the SHA1 green (vanilla peels only).

## Adding a hack (ygodm8-style)

1. Add a toggle in `configs/runtime.c` / `include/runtime.h`.
2. Implement `Name__Replacement` in `src_custom/*_hooks.c` (linked in append ROM).
3. Add a stub in `src_custom/LynJump.event` (`ORG` vanilla offset + `POIN` replacement).
4. `make` — linker places append code past 8MB; `apply_lynjump.py` patches the entry.

Target ROM: **USA/Europe**, SHA1 `132222e0c0ddacb6281cc2eb4489ed6c8719dac2`.
