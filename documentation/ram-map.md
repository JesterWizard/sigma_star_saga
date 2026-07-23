# RAM Map

## Introduction

`asm/ram_map.s` is the central address registry. It `.include`s three region sources that assemble into a single `ram_map.o`:

| File | Region |
|------|--------|
| `asm/ram_map_iwram.s` | IWRAM free pool + known vanilla symbols |
| `asm/ram_map_ewram.s` | EWRAM free pool (provisional) |
| `asm/ram_map_sram.s` | EEPROM note + unused `0x0E000000` window |

Fragments are included (not separate objects) so bump-allocator cursors stay consistent. Pattern follows [ygodm8 `ram_map.s`](https://github.com/JesterWizard/ygodm8/blob/master/asm/ram_map.s).

Custom code should reference symbols from this map instead of hard-coding `0x03…` / `0x02…` addresses.

## Region model

| Region | Range in use here | Notes |
|--------|-------------------|-------|
| IWRAM | Free `0x03007780`–`0x03007CA0` | User SP `0x03007CA0`, IRQ SP `0x03007FA0` |
| EWRAM | Free `0x02030000`–`0x02040000` | Floor is provisional until more globals are matched |
| Save | EEPROM_V124 | Not cart SRAM; see `ram_map_sram.s` |

## Macros

- `SET_DATA` — fixed absolute symbol
- `SET_ARRAY` — span with `name` / `nameEnd`
- `_kernel_malloc` / `_kernel_malloc_ewram` — downward free-space bump allocators

## Known IWRAM symbols

| Symbol | Address | Role |
|--------|---------|------|
| `gMode` | `0x03001630` | Main-loop mode |
| `gPlayerPtr` | `0x03000DB8` | Flight player object pointer |
| `gPlayerBombs` | `0x03007014` | Smart bomb count |
| `gPlayerLevel` | `0x0300702C` | RPG level |
| `gPlayerExp` | `0x0300705C` | Experience |
| `gEnemyRemaining` | `0x03007080` | Flight HUD enemy/quota digit (not HP) |
| `gExpToCurrentLevel` | `0x0300717C` | Exp floor for current level |
| `gExpToNextLevel` | `0x030071F0` | Exp to next level |
| `gStageClearFlag` | `0x03007668` | Stage-clear latch |
| `gStageClearGate` | `0x03007730` | Clear-sequence gate (0 = allow; CB Clear Stage) |
| `gGunDataBits` | `0x03007690` | Gun Data ownership bits |
| `gBulletOwned` | `0x0300773C` | Bullet Gun Data word |
| `gItemsOwned` | `0x03007740` | Key / overworld items |
| `gCannonOwned` | `0x03007744` | Cannon Gun Data word |
| `gImpactOwned` | `0x03007748` | Impact Gun Data word |

## Practical rules

1. Put shared macros in `asm/ram_map.s`.
2. Add symbols to the matching region file.
3. Prefer cursor allocators only for adjacent custom blocks.
4. Stay below the user stack in IWRAM; do not use the EEPROM cart window as if it were Flash save without verifying hardware behavior.
