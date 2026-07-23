# RAM Map

## Introduction

`asm/ram_map.s` is the central address registry. It `.include`s three region sources that assemble into a single `ram_map.o`:

| File | Region |
|------|--------|
| `asm/ram_map_iwram.s` | IWRAM free pool + known vanilla symbols |
| `asm/ram_map_iwram_pool.inc` | Auto pool inventory (`gUnk_*`, regenerate via scanner) |
| `asm/ram_map_ewram.s` | EWRAM free pool (provisional) |
| `asm/ram_map_ewram_pool.inc` | Auto pool inventory (`gUnk_*`, hits≥2) |
| `asm/ram_map_sram.s` | EEPROM note + unused `0x0E000000` window |

Fragments are included (not separate objects) so bump-allocator cursors stay consistent. Pattern follows [ygodm8 `ram_map.s`](https://github.com/JesterWizard/ygodm8/blob/master/asm/ram_map.s).

Custom code should reference symbols from this map instead of hard-coding `0x03…` / `0x02…` addresses.

## Scanning the baserom

```bash
python3 tools/scan_ram_literals.py              # report clusters / top hits
python3 tools/scan_ram_literals.py --emit-asm   # refresh asm/ram_map_*_pool.inc
```

Method: Thumb `LDR Rd,[PC,#imm]` literal pools only (not raw absolute words). Word-aligned targets preferred; EWRAM requires hits≥2 below `FreeEwramSpaceTop`.

## Occupancy (used vs safe leftover)

| Region | Span | Size | Status |
|--------|------|------|--------|
| IWRAM hardware | `0x03000000`–`0x03008000` | 32 KiB | — |
| IWRAM vanilla (pool high water) | `0x03000000`–`0x0300775C` | — | **USED** (431+ named pool symbols) |
| IWRAM pad | `0x0300775C`–`0x03007780` | 36 B | leave alone |
| IWRAM custom free | `0x03007780`–`0x03007CA0` | **1 312 B** | **SAFE** via `_kernel_malloc` |
| IWRAM user stack | `0x03007CA0`–`0x03007FA0` | 768 B | **USED** (grows down) |
| IWRAM IRQ stack | `0x03007FA0`–`0x03008000` | 96 B | **USED** |
| EWRAM hardware | `0x02000000`–`0x02040000` | 256 KiB | — |
| EWRAM dense pool use | `0x02000000`–`~0x02005E00` | — | **USED** (142 `gUnk_*`) |
| EWRAM reserved / provisional | `~0x02005E00`–`0x02030000` | — | treat as used until proven free |
| EWRAM custom free* | `0x02030000`–`0x02040000` | **64 KiB** | **SAFE*** via `_kernel_malloc_ewram*` |
| Save chip | EEPROM_V124 | — | Not `0x0E…` |
| SRAM-bus scratch** | `0x0E000000`–`0x0E000100` | 256 B | Opt-in only |

\* EWRAM free floor is provisional — raise `FreeEwramSpaceTop` only with evidence. Dense confirmed use ends near `0x02005E00`; sparse higher pool hits look like false positives.  
\*\* Not cart save; verify before use. Pool scan found no trustworthy SRAM-bus globals.

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
| `gVanillaIwramHighWater` | `0x0300775C` | Last pool-backed vanilla global |

Unknown pool-backed addresses use `gUnk_XXXXXXXX` in the `*_pool.inc` files. Promote to a named symbol in `ram_map_*.s` (+ `KNOWN_*` in the scanner) when the role is identified.

## Practical rules

1. Put shared macros in `asm/ram_map.s`.
2. Add named symbols to the matching region file; refresh pool incs with the scanner.
3. Prefer cursor allocators only for adjacent custom blocks.
4. Stay below the user stack in IWRAM; do not use the EEPROM cart window as if it were Flash save without verifying hardware behavior.
