# Data structures (JSON → ROM)

Edit JSON files in this folder. `make` compiles them into append-ROM tables
that custom hooks read at runtime (same append/`APPEND_RODATA` pattern as
dialogue). Adding or removing impacts in [`impact_data.json`](impact_data.json)
also resizes the OnImpact jump table, description table, and gun-icon ANM
buffer automatically.

| File | Toggle (`configs/runtime.c`) | Effect |
|------|------------------------------|--------|
| [`enemy_exp.json`](enemy_exp.json) | `.custom_enemy_exp` | Remap 2D flight kill EXP |
| [`impact_data.json`](impact_data.json) | `.custom_gun_data` | Custom Impact Data (text / icon / id / behavior) |

```bash
make
```

## `enemy_exp.json`

Catalog of flight / side-scroller enemies extracted from `SpawnActor` +
`InitActorParams` (EXP pool at actor `+0x3C`, gem award = pool / 10).

| Field | Meaning |
|-------|---------|
| `id` | SpawnActor type id (r2) |
| `anim` | ANM / label id |
| `name` | ROM label when known (`MB_*`, `B_*`, `EARTH_*`) |
| `exp` | Per-gem amount passed to `AddExperience` (edit to rebalance) |

Some type ids appear more than once with different `exp` (spawn variants).

## `impact_data.json`

Defines custom Impact Data pieces after the vanilla 28.

| Field | Meaning |
|-------|---------|
| `index` | Local impact index (`28` = 29th, `29` = 30th) |
| `id` | Gun Data id (`77`, `78`, …) |
| `number` | Status-screen list number |
| `name` / `text` | Status blurb → `"NAME : text"` |
| `icon_from` | Vanilla local index whose ANM frames to reuse (`27` = EXP Miner Plus) |
| `icon_png` | Authored art with badge digits |

| Piece | Effect |
|-------|--------|
| **SUCTION** | Pulls every EXP gem on screen (vanilla magnet speed) |
| **SUCTION+** | Same pull at **2×** speed |

Unlock with `.all_impact_data` and/or `.custom_gun_data` (status screen auto-unlocks
JSON pieces when `.custom_gun_data` is on). Each `icon_png` is injected as an ANM
frame pair starting at 196.
