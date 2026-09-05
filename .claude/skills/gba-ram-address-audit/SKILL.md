---
name: gba-ram-address-audit
description: Disassemble and classify an unidentified or gUnk_* IWRAM/EWRAM address in the sigma_star_saga baserom before writing code that reads or writes it. Use whenever a hook needs to touch a RAM symbol that isn't fully documented in documentation/ram-map.md, or whose gUnk_* name in asm/ram_map_iwram_pool.inc suggests its meaning was guessed rather than confirmed.
---

# GBA RAM address audit

`asm/ram_map_iwram_pool.inc` has 431+ `gUnk_*` symbols — addresses located
but never semantically confirmed. Treating one as "probably a scratch flag,
safe to zero" without checking is how a plausible-looking fix (see
rom-hook-regression-check skill) corrupts unrelated state.

## Procedure

1. **Count write sites** in the baserom (not the patched ROM — you want
   vanilla behavior):

```bash
python3 -c "
d = open('baserom.gba','rb').read()
tgt = (0x0300XXXX).to_bytes(4,'little')
hits = [i for i in range(0,len(d)-4,4) if d[i:i+4]==tgt]
print(f'{len(hits)} literal-pool refs:', [hex(0x08000000+h) for h in hits][:30])
"
```

Thumb code loads absolute addresses via `ldr rN, [pc, #off]` pointing into a
literal pool a few instructions ahead — the 4-byte constant sits inline in
the code stream, which is why a flat byte-search for the little-endian
address works without needing a disassembler pass first.

2. **Disassemble each hit site** (widen the window if the target function
   spans it):

```bash
arm-none-eabi-objdump -D -b binary -m arm \
  --start-address=0xHIT_MINUS_40 --stop-address=0xHIT_PLUS_40 \
  -Mforce-thumb baserom.gba
```

Look at what's stored: a literal immediate (`movs r0, #N; strb r0,[r1]`)
following a `cmp`/`beq` chain on some *other* register (often `gMode`) means
this is a **selector written from a dispatch table**, not a flag — the value
range matters and `0` is probably meaningful, not "empty."

3. **Classify** before writing any code against the address:

| Pattern | Meaning | Safe to zero/clear? |
|---|---|---|
| Single write site, `strb r0,[..]` with r0 always 0 or 1 | Boolean flag | Usually yes |
| Write site sets to a literal that varies with a preceding `cmp gMode,#N` | Per-context selector (0 is a valid state, not "none") | **No** — clearing corrupts state for whichever context maps to 0 |
| 10+ write sites across unrelated ROM offset ranges (different subsystems) | Shared/multi-purpose scratch, probably several logically distinct sub-fields packed together | Audit each write site's caller before touching |
| Read site does `cmp value,#K; bgt/blt` range dispatch | Enum/state-machine cursor | Only write values from the same enum, never a bare 0 unless 0 is confirmed idle |

4. **If you must update `ram-map.md`**, write down the write-site addresses
   and the value semantics you found, not just a guessed one-line label —
   the next person (or the next you, in a future session) needs the same
   audit trail so they don't have to redo the disassembly.

## Why this matters more than usual here

Runtime hooks in `src_custom/*.c` are appended ROM, not decompiled-and-
recompiled originals — they call into baserom addresses as opaque function
pointers and read/write baserom RAM layout by raw offset. There's no
compiler-checked struct definition to catch "you used the wrong field";
everything is `*(vu8 *)0x03007680`-style raw pointers. The only guardrail is
manual verification against the actual ROM bytes before trusting a symbol
name someone else picked.
