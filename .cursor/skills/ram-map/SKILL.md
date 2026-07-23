---
name: ram-map
description: >-
  Adds discovered GBA RAM globals and custom allocations to the matching
  asm/ram_map_*.s fragment and include/ram_map.h. Use whenever a new IWRAM
  (0x03…), EWRAM (0x02…), or SRAM-bus (0x0E…) address is identified from
  disassembly, decomp, or custom code, or when the user mentions RAM maps,
  SET_DATA, FreeRamSpace, or _kernel_malloc.
---

# RAM map updates

When any new RAM address or custom buffer is discovered, record it in the map before hard-coding the address in C/asm.

Guide: `documentation/ram-map.md`

## Target files

| Address range | File | Allocator |
|---------------|------|-----------|
| `0x03000000`–`0x03007FFF` | `asm/ram_map_iwram.s` | `_kernel_malloc name, size` |
| `0x02000000`–`0x0203FFFF` | `asm/ram_map_ewram.s` | `_kernel_malloc_ewram` / `_kernel_malloc_ewram_array` |
| `0x0E000000`–`0x0E00FFFF` | `asm/ram_map_sram.s` | `_kernel_malloc_flash_free` (opt-in only) |

Do not edit `asm/ram_map.s` for new symbols — it only defines `SET_DATA` / `SET_ARRAY` and `.include`s the fragments.

## Workflow

1. **Classify**
   - **Vanilla / matched absolute** → `SET_DATA name, 0x……` (or `SET_ARRAY` if size known)
   - **Custom allocation** → region `_kernel_malloc*` only (never invent a fixed free-pool address)
2. **Dedup** — grep `asm/ram_map_*.s` and `include/ram_map.h` for the name and address. Skip if already present with the same value; fix mismatches instead of duplicating.
3. **Validate**
   - Prefer Thumb literal-pool / code xrefs over absolute literals alone (EWRAM and `0x0E…` especially).
   - IWRAM: never allocate at or above user stack `0x03007CA0`; stay in free pool `0x03007780`–`0x03007CA0` for custom malloc.
   - EWRAM free floor `0x02030000` is provisional — raise `FreeEwramSpaceTop` only with evidence.
   - SRAM bus is not cart save (EEPROM_V124); only document / allocate there when explicitly needed.
4. **Insert** into the matching thematic section (or add a short `@ -- Section --` block). Keep style:
   - Hex addresses uppercase digits as in existing files (`0x03001630`)
   - Brief `@` comment when role is non-obvious
   - Custom mallocs under `@ -- Custom free-space allocations --`
5. **Mirror in C** — add `extern` to `include/ram_map.h` with the correct type (`u8` / `u16` / `u32` / pointer / array).
6. **Docs** — for newly matched vanilla IWRAM/EWRAM symbols, add a row to the known-symbols table in `documentation/ram-map.md`.
7. **Consume** — replace hard-coded addresses in new/edited code with the symbol.

## Entry templates

Vanilla absolute:

```asm
@ One-line role.
SET_DATA gExampleFlag, 0x03001234
```

Sized span:

```asm
SET_ARRAY gExampleBuf, 0x02001000, 0x100
```

Custom (pick region macro):

```asm
_kernel_malloc gExampleScratch, 0x4
_kernel_malloc_ewram gExampleBuf, 0x100
_kernel_malloc_ewram_array gExampleArray, 0x100
```

Header:

```c
extern u8 gExampleFlag;
extern u32 gExampleBuf[]; /* or sized pointer type matching use */
```

## Naming

- Prefer `g` + PascalCase (`gEnemyRemaining`, `gStageClearFlag`).
- Free-pool cursors stay as existing `Free*Space*` / `UsedFree*` names — do not rename.
- One symbol per distinct address; do not alias the same slot under two names unless documenting a known overlay.

## Done when

- [ ] Symbol lives in the correct `ram_map_*.s` fragment
- [ ] No duplicate name/address
- [ ] `include/ram_map.h` updated for C consumers
- [ ] Known vanilla symbols listed in `documentation/ram-map.md`
- [ ] Call sites use the symbol, not a raw address
