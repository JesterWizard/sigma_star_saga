---
name: gba-causal-trace-hunt
description: Find the real memory write / code path that causes an observed game-state transition (death, mode change, HUD update) in this GBA decomp, instead of assuming the address whose name or offset "sounds right." Use whenever a fix pins/vetoes one field but the symptom persists — that's the signal you're looking at a downstream effect, not the trigger.
---

# GBA causal-trace hunt

A fix that correctly stops one memory write from happening, but doesn't
stop the reported symptom, is not "probably right but incomplete" — it is
evidence you patched a *consequence*, not the *cause*. This codebase has
several one-way state machines (HP → hurt state → death sentinel → queued
mode → applied mode, with a multi-frame fade delay in between) where the
value that *looks* causal from its name or a doc comment is often several
steps downstream of the actual decision.

## The core technique: brute-force the candidate, then bisect if it fails

Before spending time reading disassembly to find "the" write site, do the
cheap experiment that tells you whether your candidate field is even on the
causal path at all:

```c
/* Force the suspected field to a "safe" value on EVERY frame, unconditionally
 * — bypassing all your own hook logic entirely. If the symptom still
 * happens, that field is not the (or not the only) trigger. */
for (int f = 0; f < N; f++) {
    core->busWrite32(core, SUSPECT_ADDR, SAFE_VALUE);
    core->setKeys(core, 0);
    core->runFrame(core);
}
```

If the bad outcome still occurs with the field pinned every single frame,
stop refining the fix around that field — it's not sufficient, no matter
how correct the pin logic is. (This is exactly what surfaced the bug this
skill was written from: `gActorPool[0]+0x34` pinned to full HP every frame
did **not** stop a game-over transition, proving the death decision reads
something else.)

## Binary-search the frame, not the code

Once you know *a* transition happens (mode flips, screen changes), find the
**exact frame** it happens on before touching disassembly:

```c
/* run to a checkpoint well before the known symptom, snapshot, step one
 * frame, diff. Repeat with a narrower window until you bracket the exact
 * frame the target address changes. */
uint8_t before = rd8(TARGET_ADDR);
core->runFrame(core);
uint8_t after = rd8(TARGET_ADDR);
if (before != after) { /* found it */ }
```

Two traps that wasted real time here:

1. **The frame a *symptom* becomes visible (mode fully applied, screen
   redraws) is often tens to hundreds of frames after the frame the
   *decision* was made.** This codebase's mode-fade path
   (`QueueModeFade` → `gPendingMode` latch → `FadeStep` applies `gMode` only
   once the fade completes) has a real, measured ~124-frame gap between
   "pending mode gets set" and "`gMode` itself changes." If you diff memory
   at the frame `gMode` changes, you will see the fade-completion
   housekeeping (actor struct zeroed by cleanup code), not the original
   decision — and conclude the wrong thing changed. Trace back through every
   intermediate latch (search for a `gPending*`-style staging field near the
   final target) to the *earliest* frame something changed, not the frame
   the visible symptom appeared.
2. **A frame number from a previous probe run does not reliably reproduce
   in a new run**, even with identical scripted input, because upstream
   boot/title timing isn't perfectly deterministic across rebuilds. Always
   re-derive "the frame it happens on" inside the same run you're using
   that number in — poll for the state change rather than hardcoding a
   frame count copied from an earlier run's printout.

## Once you have the exact frame, find every plausible writer

```bash
python3 -c "
d = open('baserom.gba','rb').read()
tgt = (0xADDR).to_bytes(4,'little')
hits = [i for i in range(0,len(d)-4,4) if d[i:i+4]==tgt]
print(len(hits), [hex(0x08000000+h) for h in hits])
"
```

A handful of hits (under ~20) is disassemble-able one by one
(`arm-none-eabi-objdump -D -Mforce-thumb --start-address=X --stop-address=Y
baserom.gba`). If the count is large (50+, e.g. a hot global like `gMode`
itself), don't disassemble every site — instead diff the *decompressed*
IWRAM (not just the one target address) across the bracketed frame and look
for what *else* changed alongside it; the accompanying writes usually
narrow which function was actually running that frame far faster than
reading 50 call sites cold.

## Watch for a visible-effect correlate, but don't trust it blindly

A cheap "did the on-screen thing change" signal (sum of pixel values in a
screen region, byte-diff of a sprite's attribute table) is useful for
spotting *when* to look closer, but idle animations (palette cycling, a
pulsing HUD bar, a walk-cycle frame) produce the exact same kind of
periodic, evenly-spaced "change" a real symptom would. Before trusting a
pixel-sum trigger as "the HP bar dropped," pull the actual screenshot at
that frame and look at it — a perfectly regular decrement every 2 frames is
a shimmer effect, not damage; real damage is bursty and irregular.

## When a hook you just added doesn't fix the symptom

Confirm the hook is even *reachable* before doubting its logic:

1. Dump the raw bytes at the patched ROM address
   (`xxd -s 0xOFFSET -l 16 file.gba`) and manually decode the veneer —
   confirm it's a `ldr r3,[pc,#0]; bx r3` (or whatever the veneer pattern
   is) pointing at your replacement symbol's real, current address (re-run
   `nm` — addresses shift between builds).
2. If the veneer is confirmed correct and installed, but the symptom
   persists, the most likely explanation is not "the veto logic is wrong,"
   it's "this specific code path doesn't call through the function you
   hooked at all." A game engine frequently has more than one entry point
   into "the same" logical operation (a boss's instant-kill collision may
   never call the generic damage-application routine the normal weapon-hit
   path uses). Re-run the frame-bisection above specifically for *this*
   attack/scenario rather than assuming the mechanism found for a different
   scenario generalizes.

## Report the tier honestly

See the `verification-scope-honesty` skill for how to phrase what you've
actually confirmed once you stop — "the field I found is pinned correctly
and verified via A/B savestate test" is a different (weaker) claim than
"the reported symptom no longer occurs," and conflating them after a long
investigation is the easiest place to overclaim.
