---
name: verification-scope-honesty
description: How to report fix status when a bug's root cause is confirmed and repaired but the end-to-end gameplay symptom couldn't be observed to stop (e.g. requires beating a level, RNG-gated content, or other real gameplay skill a scripted emulator probe can't perform). Use when a debugging task's Definition of Done implies watching a symptom disappear in actual play, not just a code-path/unit-level check.
---

# Verification scope honesty

Some bugs in this repo (RNG-gated random encounters, story-locked boss
fights, anything past a shooter stage that must be *won*) can be root-caused
and fixed with full confidence from disassembly + targeted A/B tests, while
the *symptom itself* — "does the tutorial stop repeating," "does the
encounter fire correctly" — can only be observed by someone actually playing
skillfully, not by a scripted `core->setKeys()` loop.

## What counts as sufficient evidence, ranked

1. **Best**: watched the exact reported symptom stop in the emulator via
   scripted or manual play that reaches the real trigger condition.
2. **Good**: A/B test through the *actual UI path* (real menu navigation,
   real toggle) that isolates the corrupted state and shows broken-build
   corrupts it / fixed-build preserves it, with a negative control (bug
   reintroduced → test fails again).
3. **Weaker but often the ceiling**: root cause confirmed by disassembly +
   the fix demonstrably stops the specific write/read that was wrong, but
   the downstream gameplay symptom was never directly observed to resolve.

Tier 3 is real, valuable work — it is not "no progress." But it is not the
same claim as tier 1, and conflating them is the failure mode to avoid.

## What to do when you're stuck at tier 2/3 and can't reach tier 1

- **State the tier explicitly** in your own words when reporting: "the
  defect is fixed and verified via [tier 2 test]; I was not able to observe
  [exact symptom] stop in actual play because [concrete blocker: requires
  surviving X, warping doesn't spawn the player, etc.]."
- **Try to find independent evidence the blocker is pre-existing**, not
  caused by your change — e.g. build the pre-fix commit and show the same
  automated-play limitation exists there too, or check that the unmodified
  baserom has the identical limitation. This converts "I couldn't verify"
  into "I couldn't verify, and here's proof that's a harness limit, not a
  gap in the fix."
- **Hand back a concrete unblocking step** the user can do in seconds that
  you cannot do yourself: "run this build and walk into an encounter" or "a
  savestate taken while standing on a planet lets me finish verification
  myself." Don't just report the gap — close as much of the distance as
  possible and name exactly what's left.
- **Do not write a stronger claim than you've earned** even under repeated
  pressure to close out a task/goal/hook condition. If asked "is it fixed?"
  and the true answer is "the defect is fixed, the symptom is unconfirmed,"
  say that plainly every time, without escalating to "it's fixed" just
  because the question repeats. A false confirmation is worse than an
  honest incomplete — it causes the user to stop checking something that
  might still be broken.
- **Recognize when further automated attempts have stopped producing new
  information.** If you've tried N structurally different approaches to
  reach the same observation and each fails for a diagnosed (not
  mysterious) reason, a to-N+1th attempt using the same class of tooling
  (another scripted key-mashing variant) is unlikely to succeed either.
  Say so, rather than looping indefinitely on cosmetic variations of the
  same blocked approach — but do keep trying genuinely different strategies
  (different game-entry paths, different debug features, invincibility
  cheats, direct state manipulation) before concluding it's blocked, since
  an early "I can't verify this" can itself be wrong if a different
  approach would have worked.

## Applies beyond this repo

The general shape — "root cause fixed and unit/path-tested, but the
end-user-visible symptom requires an environment or skill the agent doesn't
have access to" — recurs in any debugging task with a manual-reproduction
step (a UI bug that needs a real browser interaction sequence, a race
condition that needs real concurrent load, a hardware-adjacent bug that
needs the physical device). The reporting discipline is the same regardless
of domain: name the tier, name the blocker, prove it's not your fix's fault
where possible, and hand back the smallest possible unblocking step.
