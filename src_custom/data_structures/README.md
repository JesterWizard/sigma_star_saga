# Data structures (JSON → ROM)

Edit JSON files in this folder. `make` compiles them into append-ROM tables
that custom hooks read at runtime (same append/`APPEND_RODATA` pattern as
dialogue).

| File | Toggle (`configs/runtime.c`) | Effect |
|------|------------------------------|--------|
| [`enemy_exp.json`](enemy_exp.json) | `.custom_enemy_exp` | Remap 2D flight kill EXP |

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
