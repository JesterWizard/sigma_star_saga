"""Regression coverage for the Ian Recker "stuck climbing pose" bug.

Root cause (see documentation/debug-menu.md, "Ship picker -- removed,
incident record"): a debug-menu per-frame flight hook force-wrote
gActorPool[0]+0x22 (ACTOR_OFF_MODEL -- a general model/animation-index field
reused by overworld/cutscene actors, NOT player-exclusive) from inside
UpdateShooterFrame__Replacement, whose call site is shared by ~129 gMode
values, not flight-exclusive. The write persisted across a mode transition
and got misread as a bogus animation index by whatever actor next occupied
pool slot 0, freezing the player mid-animation.

These cases assert the general invariant the incident violated: reaching a
walkable overworld field mode after a flight stage must never leave
gActorPool[0]'s model/animation field holding a flight-stage value, and the
player must be able to move (proof the walk-update path isn't stuck).
"""
from __future__ import annotations

from pathlib import Path

from ..boot import fresh_boot_to_field_mode, is_field_mode
from ..harness import KEY_DOWN, Harness
from ..symbols import SymbolTable
from ..testcase import TestCase

ACTOR_OFF_MODEL = 0x22
ACTOR_STRIDE = 0x60


def _case_field_actor_model_not_flight_residue(h: Harness, symbols: SymbolTable) -> None:
    gmode = symbols.addr("gMode")
    actor_pool = symbols.addr("gActorPool")

    mode = fresh_boot_to_field_mode(h, gmode, symbols)
    assert is_field_mode(mode), f"expected a walkable field mode, got {mode:#04x}"

    # Let the field's own actor-init settle for a couple frames before
    # sampling -- avoids reading a value mid-transition.
    h.run(5, 0)

    # Known-bad flight-stage-only model values (the HP-tier stage-type codes
    # the removed ship picker used to force into this same offset). If any
    # pool slot still carries one of these after we're on a real field mode,
    # something is leaking flight-stage state across the mode boundary again.
    known_bad_flight_values = {0x0002, 0x0042}

    for slot in range(4):  # first few slots cover the player + nearby actors
        addr = actor_pool + slot * ACTOR_STRIDE + ACTOR_OFF_MODEL
        value = h.read16(addr)
        assert value not in known_bad_flight_values, (
            f"gActorPool[{slot}]+0x22 == {value:#06x} on field mode "
            f"{mode:#04x} -- looks like a flight-stage-only model/stage-type "
            "value leaked into an overworld actor slot (the Ian Recker "
            "stuck-pose bug's exact mechanism). Did a per-frame flight hook "
            "start writing ACTOR_OFF_MODEL again without a real flight-mode "
            "gate?"
        )


def _case_player_can_move_on_field(h: Harness, symbols: SymbolTable) -> None:
    """Proof the walk-update path isn't stuck: position must change under input.

    This is the direct symptom check for "stuck in a pose the player can't
    break out of" -- if held DOWN never moves the tracked actor, whatever
    broke movement/animation state is still broken, regardless of what any
    single memory offset reads.
    """
    gmode = symbols.addr("gMode")
    actor_pool = symbols.addr("gActorPool")
    actor_off_y = 0x4C  # see include/actor.h ACTOR_OFF_Y

    mode = fresh_boot_to_field_mode(h, gmode, symbols)
    assert is_field_mode(mode), f"expected a walkable field mode, got {mode:#04x}"
    h.run(10, 0)

    y_before = h.read32(actor_pool + actor_off_y)
    h.run(60, KEY_DOWN)
    y_after = h.read32(actor_pool + actor_off_y)

    assert y_before != y_after, (
        f"gActorPool[0]+0x4C (Y position) did not change after holding DOWN "
        f"for 60 frames on field mode {mode:#04x} (before={y_before:#010x} "
        f"after={y_after:#010x}) -- the player appears stuck and unable to "
        "move, matching the Ian Recker stuck-pose symptom."
    )


def _case_debug_menu_opens_on_field(h: Harness, symbols: SymbolTable) -> None:
    """Sanity check the debug menu itself still opens on a real field mode.

    Not directly about the incident, but cheap here (same boot) and catches
    any future regression to IsOverworldFieldMode()'s gating.
    """
    gmode = symbols.addr("gMode")
    active = symbols.addr("gDebugMenuActive")

    mode = fresh_boot_to_field_mode(h, gmode, symbols)
    assert is_field_mode(mode), f"expected a walkable field mode, got {mode:#04x}"
    h.run(10, 0)

    from ..harness import KEY_START

    h.run(4, KEY_START)
    h.run(10, 0)
    is_active = h.read8(active)
    assert is_active != 0, (
        f"debug menu did not open on field mode {mode:#04x} after START "
        "-- IsOverworldFieldMode() gating may have regressed"
    )


CASES = [
    TestCase(
        name="field_actor_model_not_flight_residue",
        fn=_case_field_actor_model_not_flight_residue,
        description=(
            "No overworld actor slot's model/animation field (+0x22) holds a "
            "flight-stage-only value after reaching a field mode -- direct "
            "regression check for the Ian Recker stuck-pose bug."
        ),
    ),
    TestCase(
        name="player_can_move_on_field",
        fn=_case_player_can_move_on_field,
        description=(
            "Holding DOWN on a field mode actually moves the tracked actor "
            "-- direct symptom check for 'stuck in a pose' style regressions."
        ),
    ),
    TestCase(
        name="debug_menu_opens_on_field",
        fn=_case_debug_menu_opens_on_field,
        description="START opens the debug menu on a real walkable field mode.",
    ),
]
