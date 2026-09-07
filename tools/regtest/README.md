# tools/regtest — build-time gameplay regression suite

Boots the just-built ROM under headless mGBA and asserts gameplay
invariants that a bad hook (bad memory write, missing mode gate, wrong
struct offset) can silently break without any compiler error catching it.

This suite exists because of a real incident: a debug-menu "ship picker"
feature force-wrote an actor-struct offset every frame from a hook whose
call site is shared across ~129 `gMode` values, not flight-exclusive. It
compiled cleanly, looked correct on inspection, and broke the player
character (stuck in an unrecoverable pose) the moment the feature was
actually exercised. See `documentation/debug-menu.md`, "Ship picker —
removed, incident record," for the full writeup.

## Running

```sh
make test          # builds the ROM + harness, then runs every case
make regtest        # same thing, different name
python3 -m tools.regtest.run_regtests            # skip the ROM (re)build
python3 -m tools.regtest.run_regtests -k ship     # filter by module.case name
```

Requires `sigma_star_saga.gba` and `sigma_star_saga.elf` to already be
built (`make` does this before running tests). The harness binary
(`tools/regtest_harness`) is built automatically against the prebuilt
mGBA 0.9.3 headers/libs at `/home/username/tools/mgba` — see
`.claude/skills/mgba-headless-probe` for why that specific build is used
for fresh-boot probing (a different, newer mGBA build is needed only for
loading real user savestates, which this suite never does).

## Layout

- `regtest_harness.c` (in `tools/`, not this package) — generic,
  script-driven mGBA wrapper. Understands `RUN`, `RD8/16/32`, `WR8/16/32`,
  `RUN_UNTIL8`, `SHOT`, nothing test-specific. Never hardcodes a game
  symbol; every address a test needs comes from the Python side.
- `harness.py` — Python subprocess wrapper around the C binary.
- `symbols.py` — resolves EWRAM/IWRAM symbol addresses fresh from the
  just-built `.elf` via `arm-none-eabi-nm`. **Never hardcode an address in
  a test case** — debug-menu EWRAM symbols in particular shift between
  builds (bump-allocated in `asm/ram_map_ewram.s`); always go through
  `SymbolTable.addr("gSomeSymbol")`.
- `boot.py` — shared, proven boot sequences (fresh boot → intro flight →
  first walkable field mode). Add new proven sequences here instead of
  hand-rolling a new mash loop inside a test case; that's how the
  `mgba-headless-probe` skill documents avoiding hours of rediscovery.
- `testcase.py` — the plain `TestCase(name, fn, description)` shape every
  case module returns as `CASES`.
- `cases/*.py` — the actual regression assertions, one module per feature
  area. `run_regtests.py` discovers every module here automatically.
- `run_regtests.py` — CLI entry point / summary printer.

## Writing a new test case

1. Add or extend a module under `cases/`. Each module exports a module-level
   `CASES: list[TestCase]`.
2. A case function has the signature `fn(h: Harness, symbols: SymbolTable) -> None`.
   Assert with plain `assert` — a failure is just an `AssertionError`, and the
   runner reports it with the message you wrote. Write that message to name
   the actual invariant broken, not just "test failed" — the whole point of
   this suite is that a future regression should be diagnosable from the
   assertion message alone, the way the ones in `debug_menu_actor_safety.py`
   name the exact offset/symptom from the incident they guard against.
3. Boot with `h.boot(ROM_PATH)` (the runner does this once per case, fresh —
   cases must not depend on state left over from another case) then drive
   input via `h.run(frames, keys)` / the shared helpers in `boot.py`.
4. Prove the new case actually discriminates before trusting it: temporarily
   reintroduce the bug it's meant to catch (or comment out the fix), rerun
   `python3 -m tools.regtest.run_regtests -k your_case`, confirm it fails,
   then revert the temporary breakage. A test that can't fail isn't testing
   anything — this mirrors the "negative control" step in
   `.claude/skills/gba-causal-trace-hunt`.

## Test-driven workflow for new features/fixes (required — see CLAUDE.md)

Per the project's TDD rule: for any new debug-menu entry, per-frame hook, or
runtime-config toggle, write (or extend) a `regtest` case that encodes the
invariant the change must not break *before* writing the implementation.
Run it once to confirm it fails against the current (unfixed) tree, then
implement until it passes. This is exactly the discipline that would have
caught the ship-picker regression before it shipped: the corruption was
only visible by actually running the game past the point where the hook's
call site diverges from "real flight," which nothing short of a scripted
probe reliably exercises on every change.
