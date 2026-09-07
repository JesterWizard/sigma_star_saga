"""Shared boot sequences for regression tests.

Reuses the proven fresh-boot input scripts documented in
.claude/skills/mgba-headless-probe/SKILL.md ("Proven boot sequences (stop
rediscovering these)") -- do not hand-roll a new mash loop per test case,
add a new proven sequence here instead so every test benefits.
"""
from __future__ import annotations

from .harness import Harness, KEY_A, KEY_L, KEY_SELECT, KEY_START

# Walkable overworld field gMode ranges (see include/overworld_frame.h).
FIELD_MODE_RANGES = ((0x04, 0x09), (0x0F, 0x17))


def is_field_mode(mode: int) -> bool:
    return any(lo <= mode <= hi for lo, hi in FIELD_MODE_RANGES)


def fresh_boot_to_intro_flight(h: Harness) -> None:
    """Splash -> title -> story intro -> lands in the opener flight stage.

    Matches the skill's documented ~1131-frame mash + 122 dialogue-advance
    taps. Ends with gMode == 0x97 or 0x84 (intro flight), NOT a field mode.
    """
    for frame in range(1131):
        ph = frame % 40
        keys = 0
        if ph < 4:
            keys = KEY_A
        elif 10 <= ph < 14:
            keys = KEY_START
        elif 20 <= ph < 30:
            keys = KEY_SELECT | KEY_L
        h.run(1, keys)

    for _ in range(122):
        h.run(3, KEY_A)
        h.run(20, 0)


def fresh_boot_to_field_mode(
    h: Harness, gmode_addr: int, symbols=None, max_extra_frames: int = 6000
) -> int:
    """fresh_boot_to_intro_flight, then push through to a walkable field mode.

    Uses the SELECT+L flight-skip cheat plus the same stage-clear-flag force
    tools/mgba_debug_menu_probe.c relies on (SELECT+L alone does not
    reliably end the Ch.1 opener flight stage within a short window) so the
    game hands control to a real field mode. Pass `symbols` (a SymbolTable)
    to resolve gStageClearFlag/gEnemyRemaining/gStageClearGate; without it,
    only the SELECT+L cheat is used and boot may be slower/less reliable.

    Returns the gMode reached; raises AssertionError if a field mode is
    never reached.
    """
    fresh_boot_to_intro_flight(h)

    clear_flag = symbols.addr("gStageClearFlag") if symbols else None
    enemy_remaining = symbols.addr("gEnemyRemaining") if symbols else None
    clear_gate = symbols.addr("gStageClearGate") if symbols else None

    reached_mode = None
    for frame in range(max_extra_frames):
        ph = frame % 40
        keys = 0
        if ph < 4:
            keys = KEY_SELECT | KEY_L
        elif 10 <= ph < 13:
            keys = KEY_A
        h.run(1, keys)
        mode = h.read8(gmode_addr)
        if mode in (0x84, 0x97) and clear_flag is not None:
            h.write8(clear_flag, 1)
            h.write8(enemy_remaining, 0)
            h.write8(clear_gate, 0)
        if is_field_mode(mode):
            reached_mode = mode
            break

    assert reached_mode is not None, (
        f"never reached a walkable field mode within {max_extra_frames} "
        "frames after the intro -- boot sequence may need updating"
    )
    return reached_mode
