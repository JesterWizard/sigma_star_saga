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
- `.all_key_items` — full `gItemsOwned` (CB “Have All Items”: flares, fossils, key bits, …)
- `.all_tools` — Krill Puck, Krill Boots, Girl Wings, Scanner, Warp Tool (same event-flag slide as CB “Have All Tools”, including flag 24 so L/R cycling works)

### Custom dialogue (hack)

Toggle `.custom_dialogue` in [`configs/runtime.c`](configs/runtime.c). When `TRUE` (default), edit scenes under [`src_custom/dialogue/`](src_custom/dialogue/) and `make` — talk banks are compiled into append ROM and the vanilla bank pointer table is redirected. See [`documentation/dialogue/`](documentation/dialogue/).

### Data structures / enemy EXP (hack)

JSON tables under [`src_custom/data_structures/`](src_custom/data_structures/) compile into append ROM. [`enemy_exp.json`](src_custom/data_structures/enemy_exp.json) is a by-id catalog (`.custom_enemy_exp`); gem awards use the full actor EXP pool, scaled only by `.exp_multiplier`.

### Suction Impact Data (hack)

Toggle `.custom_gun_data` in [`configs/runtime.c`](configs/runtime.c). When `TRUE` (default), loads custom Gun Data from JSON: Impact ([`impact_data.json`](src_custom/data_structures/impact_data.json) — **Suction** / **Suction+** / **Phoenix** / **Training Weights**), Cannon ([`cannon_data.json`](src_custom/data_structures/cannon_data.json) — **AUTO TARGET**), Bullet ([`bullet_data.json`](src_custom/data_structures/bullet_data.json) — **CHARGE SHOT** override + **LASER** pierce). Unlocked with `.all_*_data` / `.custom_gun_data`. Note: `.always_max_health` prevents death, so Phoenix will not trigger while that cheat is on.

### Enemy HP bars (hack)

Toggle `.enemy_hp_bars` in [`configs/runtime.c`](configs/runtime.c). When `TRUE` (default), draws HP bars under flight enemies via soft OAM after `DrawActors`. Max HP is snapshotted in `InitActorParams`. Decomps: [`src/actor.c`](src/actor.c), [`src/actor_draw.c`](src/actor_draw.c).

### Disable random battles (hack)

Toggle `.disable_random_battles` in [`configs/runtime.c`](configs/runtime.c). When `TRUE` (default), no-ops `TryStartRandomBattle` (`0x1DA5C`) — the same path the CodeBreaker “no random battles” cheat gates via `gRandomBattleCooldown` (`0x03007684`). Lure circles (`ScanEncounters`) and scripted battles stay vanilla.

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
| `src_custom/` | Hack hooks (`*_hooks.c`) + `LynJump.event` + editable `dialogue/` / `data_structures/` |
| `configs/runtime.c` | Boolean / value toggles for hacks |
| `data/dialogue/` | Vanilla talk-script dump (regenerable reference) |
| `documentation/` | Feature docs (`ram-map.md`, `dialogue/`, …) |
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
