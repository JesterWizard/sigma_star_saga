# Data structures (JSON → ROM)

Edit JSON files in this folder. `make` compiles them into append-ROM tables
that custom hooks read at runtime (same append/`APPEND_RODATA` pattern as
dialogue). Adding or removing pieces in [`impact_data.json`](impact_data.json),
[`cannon_data.json`](cannon_data.json), or [`bullet_data.json`](bullet_data.json)
also resizes the matching dispatch jump table, description table, and the shared
gun-icon ANM buffer automatically.

| File | Toggle (`configs/runtime.c`) | Effect |
|------|------------------------------|--------|
| [`enemy_exp.json`](enemy_exp.json) | `.custom_enemy_exp` | Remap 2D flight kill EXP |
| [`impact_data.json`](impact_data.json) | `.custom_gun_data` | Custom Impact Data (text / icon / id / behavior) |
| [`cannon_data.json`](cannon_data.json) | `.custom_gun_data` | Custom Cannon Data (text / icon / id / firing pattern) |
| [`bullet_data.json`](bullet_data.json) | `.custom_gun_data` | Bullet Data overrides + custom appends (text / icon / id / shot) |

```bash
make
```

## `enemy_exp.json`

Catalog of flight / side-scroller enemies from `SpawnActor` + `InitActorParams`.
The gem stores the full EXP pool at actor `+0x3C` and `AddExperience` receives
that same value (not pool/10).

| Field | Meaning |
|-------|---------|
| `id` | SpawnActor type id (r2) |
| `anim` | ANM / label id |
| `name` | ROM label when known (`MB_*`, `B_*`, `EARTH_*`) |
| `exp` | Full pool / `AddExperience` amount (must match actor `+0x3C`) |

`.custom_enemy_exp` builds the by-id catalog only. Award scaling at runtime is
`.exp_multiplier` alone — do not treat this JSON as an amount remap table.
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
| **PHOENIX** | One full HP revival when dying, once per battle |

Unlock with `.all_impact_data` and/or `.custom_gun_data` (status screen auto-unlocks
JSON pieces when `.custom_gun_data` is on). Each `icon_png` is injected as an ANM
frame pair starting at 196.

## `cannon_data.json`

Defines custom Cannon Data pieces after the vanilla 28.

| Field | Meaning |
|-------|---------|
| `index` | Local cannon index (`28` = 29th) |
| `id` | Gun Data id (`80`, …) |
| `number` | Status-screen list number |
| `name` / `text` | Status blurb → `"NAME : text"` (max 0x50 bytes) |
| `icon_from` | Vanilla local index whose ANM frames to reuse (`27` = Mirror Node) |
| `fire_from` | Vanilla local index whose OnCannon handler to reuse (`1` = Rapid Cannon) |
| `icon_png` | Authored art with badge digits |

| Piece | Effect |
|-------|--------|
| **AUTO TARGET** | Fires the Rapid Cannon pattern; shots bend toward the nearest enemy |

Unlock with `.all_cannon_data` and/or `.custom_gun_data`. Cannon icon frame pairs
follow the impact pairs in the same extended ANM (196 + 2 × impact count + 2 × i).

## `bullet_data.json`

Defines Bullet Data pieces: **vanilla overrides** (`index` 0..19) and **appended**
customs (`index` ≥ 20).

| Field | Meaning |
|-------|---------|
| `index` | Local bullet index (`3` = CHARGE SHOT rework; `20` = 21st piece) |
| `id` | Gun Data id (`32` = vanilla Charge; `81` = LASER, …) |
| `number` | Status-screen list number |
| `name` / `text` | Status blurb → `"NAME : text"` (max 0x46 bytes) |
| `icon_from` | Vanilla local index whose ANM frames to reuse (appends; optional on overrides) |
| `shot_from` | Vanilla local index whose OnBullet handler to reuse (appends; optional on overrides) |
| `icon_png` | Authored art with badge digits (**required for appends**, omit for overrides) |

| Piece | Effect |
|-------|--------|
| **CHARGE SHOT** (override 3) | Auto-charges (~2s), then **10×** fire for 3s, then charges again |
| **LASER** | Normal Shot stream as a pierce beam (period 2; vanilla 10-shot ring) |

Overrides only replace the status description (and document the rework); they do
not grow the OnBullet jump table or gun-icon ANM. Appends unlock with
`.all_bullet_data` and/or `.custom_gun_data`. Bullet icon frame pairs for appends
follow the cannon pairs in the same extended ANM
(196 + 2 × (impact count + cannon count) + 2 × i).
