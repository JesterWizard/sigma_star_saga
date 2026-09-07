# Agent notes

Read **[ARCHITECTURE.md](ARCHITECTURE.md)** first — it explains the
decompile-and-append build model, the directory layout, and the "two kinds
of 'off'" trap that causes most wasted debugging time in this repo (a
feature can be absent from the ROM at the build-patching step even though
its C code compiles and its debug-menu UI looks wired up).

## Where things are documented

| Question | Look here |
|---|---|
| "What does this file/directory do?" | [ARCHITECTURE.md](ARCHITECTURE.md) |
| "What does this RAM address do?" | [documentation/ram-map.md](documentation/ram-map.md), then `.claude/skills/gba-ram-address-audit` if it's a `gUnk_*` symbol not yet documented there |
| "How does the debug menu / a specific runtime toggle work?" | [documentation/debug-menu.md](documentation/debug-menu.md) — also the best model in this repo for how to write up a bug's root cause, what was tried and reverted, and what's still open |
| "How does GAX audio integration work?" | [documentation/gax-audio.md](documentation/gax-audio.md) |
| "What's shipped vs. still planned?" | [README.md](README.md) (shipped hacks, one section per feature) and [TODO.md](TODO.md) (open work) |
| "How do I run a headless mGBA probe / load a savestate?" | `.claude/skills/mgba-headless-probe` |
| "A fix doesn't stop the symptom, how do I find the real cause?" | `.claude/skills/gba-causal-trace-hunt` |
| "How do I report a fix that's verified at the code level but not the full gameplay level?" | `.claude/skills/verification-scope-honesty` |
| "What should I check before trusting a new LynJump/veneer hook?" | `.claude/skills/rom-hook-regression-check` |

## Non-obvious defaults worth knowing up front

- `configs/runtime.c` is the build-time source of truth for which cheats
  exist in the ROM **at all**. A debug-menu toggle can only ever suppress or
  enable behavior a build-time flag already made possible — check
  `tools/apply_lynjump.py`'s matching `apply_*` function's install
  condition before assuming a toggle's logic is wrong.
- Two mGBA builds exist locally for headless probing: the prebuilt
  `/home/username/tools/mgba` (fast, but too old to load savestates from the
  user's real desktop mGBA app) and the source checkout at
  `~/Github/mgba-orchestral` (needed for that). See the
  `mgba-headless-probe` skill before writing a probe that needs to load a
  user-provided `.ss*` file.
- Never point a probe, or any script, at the real `sigma_star_saga.sav` or
  `sigma_star_saga.ss*` files — always `cp` to a scratch path first. This is
  called out in the skill but is easy to forget mid-investigation.
- This project's own documentation (`documentation/debug-menu.md`'s
  "Limitations & Bugs" section especially) already records several
  investigations that hit real dead ends, including exact root causes,
  what was tried, and why it didn't work. Read the relevant section before
  re-investigating something that looks like a known open issue — it may
  already be documented as unsolved, with the reasoning for why.
