---
name: rom-hook-regression-check
description: Checklist for auditing a LynJump/veneer hook (IsOverworldFieldMode-style mode gates, RestoreWalk-style state clearers, polarity-tested flags) in this GBA decomp before trusting it. Use when a debug-menu feature, cheat toggle, or runtime-config flag added in a recent commit is suspected of causing a regression elsewhere in the game.
---

# ROM hook regression check

This codebase veneers ROM functions via LynJump sites (`tools/apply_lynjump.py`)
and gates custom behavior on hand-written predicates like
`IsOverworldFieldMode()` / `IsCutsceneActive()`. Three failure patterns kept
recurring across otherwise-correct-looking commits. Check for all three
before trusting a new hook.

## 1. Jump-table membership tests are almost always too broad

`gMode`'s main-loop JT is a many-to-one mapping — **129 of ~150 mode slots
point at the same `OverworldMainFrame` handler** in this ROM, including the
status/save panel (`0x8F`) and both 2D flight stages (`0x84`/`0x97`), none of
which are walkable fields. A predicate that only checks
`jt[mode-1] == OVERWORLD_FRAME_ADDR` will fire on all of them.

**Check**: does the guard also range-test `gMode` against the actual
walkable set (`0x04`-`0x09`, `0x0F`-`0x17` in this ROM — see
`include/overworld_frame.h`)? If not, any code gated on "is this a field
mode" will run somewhere it shouldn't — often invisibly, because the visible
symptom (a stale display, a corrupted selector) shows up on a *different*
screen than the one you're testing.

Reproduce with the mgba-headless-probe skill: force `gMode` to each
suspect value directly (`core->busWrite8(core, 0x03001630, m)`), let it
settle ~90 frames, then check whether your hook's guard function returns
true when it shouldn't.

## 2. Boolean flags described by a comment can have the polarity backwards

`IsCutsceneActive()` was written as `gActorCtrlLock != 0`, with a plausible
comment about a specific cutscene freeze it was patching around. The actual
polarity was inverted — that byte means *player control enabled*, confirmed
by disassembling the one write site (`Helper_2CD40`) and the one read site
(the walk-input primer at `0x0802C81C`) and checking which branch skips
which work.

**Check**: for any single-byte engine flag a hook reads but didn't write, do
not trust the comment's naming — verify empirically. Sample the byte across
~30000 real gameplay frames spanning multiple field modes:

```c
for (int i = 0; i < 30000; i++) { core->runFrame(core); if (byte) held++; }
printf("held %d/30000 frames\n", held);
```

If a "should be rare" flag is 100% or 0% the whole time, the polarity (or
the whole premise of what the byte tracks) is wrong. A flag that's supposed
to gate rare events but is always true means every check against it is
either a no-op or a permanent block — both are bugs worth finding before
they ship.

## 3. "Restore/clear" helpers must not touch bytes they don't own

`RandomBattle_RestoreWalk()` zeroed three IWRAM bytes it labeled
`OW_ENC_MODE` / `OW_ENC_SLOT` / `OW_ENC_PENDING` to "reset" encounter state.
Those addresses turned out to be a **per-planet selector the engine sets
from `gMode`** (documented nowhere but the disassembly at the site that
writes it), where `0` is a *valid value for one specific planet*, not
"cleared" or "none." Zeroing it silently redirected every subsequent lookup
to that planet's data table.

**Check**: for every address a cleanup/restore function writes, find every
write site in the ROM (not just the one your new code added) and confirm
what the full value range means:

```bash
python3 -c "
d = open('baserom.gba','rb').read()
tgt = (0xADDR).to_bytes(4,'little')
hits = [i for i in range(0,len(d)-4,4) if d[i:i+4]==tgt]
print([hex(0x08000000+h) for h in hits])
"
```

A high hit count (10+) across functions in unrelated subsystems is a strong
signal the address is shared/multi-purpose state, not a private scratch
byte safe to zero. Disassemble a few hit sites
(`arm-none-eabi-objdump -D -b binary -m arm --start-address=X --stop-address=Y
-Mforce-thumb baserom.gba`) to see what value is actually stored and under
what condition — if it varies by `gMode`/`cmp` dispatch rather than being a
flat 0/1 toggle, it's a selector, not a boolean, and "restore to 0" is wrong.

## Fast A/B verification once you have a suspected fix

Don't just read the patched code and reason about it — build both versions
and diff observable state through the *real* code path (the actual menu, the
actual toggle), not by calling the internal function directly:

1. Seed the shared state to a non-degenerate value (e.g. `busWrite8(addr, 1)`
   where `0` would be indistinguishable from "already cleared").
2. Drive the real UI sequence (open menu, navigate to row, press A) via
   `core->setKeys()` + `core->runFrame()` loops — not a raw function call —
   so the test also covers wiring, not just logic.
3. Read the state back and compare broken-vs-fixed builds side by side.
4. Temporarily reintroduce the bug (`git diff` it back in, rebuild) as a
   negative control — if the "fixed" test also passes on the broken build,
   your test isn't discriminating anything.
