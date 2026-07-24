# Data structures (JSON → ROM)

Edit JSON files in this folder. `make` compiles them into append-ROM tables
that custom hooks read at runtime (same append/`APPEND_RODATA` pattern as
dialogue).

| File | Toggle (`configs/runtime.c`) | Effect |
|------|------------------------------|--------|
| [`enemy_exp.json`](enemy_exp.json) | `.custom_enemy_exp` | Remap 2D flight kill EXP |
| [`impact_suction.json`](impact_suction.json) | `.custom_suction_impact` | 29th Impact **Suction** (text / icon / id) |

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

## `impact_suction.json`

Defines the custom **Suction** Impact Data piece (full-screen EXP magnet).

| Field | Meaning |
|-------|---------|
| `index` | Local impact index (`28` = 29th piece) |
| `id` | Gun Data id (`77`) |
| `number` | Status-screen list number (`29`) |
| `name` / `text` | Status blurb → `"NAME : text"` |
| `icon_from` | Vanilla local index whose ANM frames to reuse (`27` = EXP Miner Plus) |
| `icon_png` | Authored art with badge **29** (`graphics/gun_data/impact/29_suction.png`) |

Unlock with `.all_impact_data` (also auto-unlocked on the status screen when
`.custom_suction_impact` is on). Custom badge art comes from `icon_png` and is
injected as ANM frames 196/197.
