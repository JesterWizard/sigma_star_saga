#!/usr/bin/env python3
"""Verify Phoenix revive hooks in the built ROM.

Phoenix revives inside PlayerDeathFx, then PhoenixDeathFxSkipReturn remaps LR
past the caller's DeleteActor. DeleteActor may be veneered for overworld EXP
awards, but the replacement must always Continue — skipping softlocks the
overworld (save plug).
"""

from __future__ import annotations

import struct
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ROM = ROOT / "sigma_star_saga.gba"
ELF = ROOT / "sigma_star_saga.elf"

DEATH_FX_OFF = 0x1BA4C
DELETE_ACTOR_OFF = 0x6310
VENEER_HEAD = struct.pack("<HH", 0x4B00, 0x4718)
IWRAM_STACK_FLOOR = 0x03007CA0
DEATH_SKIP_RETURNS = [
    (0x080215DD, 0x0802160D),
    (0x08022737, 0x08022767),
    (0x08023225, 0x080232AF),
    (0x0802367B, 0x080236A3),
    (0x08023A89, 0x08023B77),
    (0x08023E99, 0x08023EC1),
    (0x08024105, 0x0802412D),
    (0x08024631, 0x080246A7),
    (0x08024927, 0x0802494F),
    (0x08024D9D, 0x08024E15),
    (0x080251D9, 0x08025243),
    (0x0802535D, 0x08025385),
]


def load_symbols() -> dict[str, int]:
    output = subprocess.check_output(["arm-none-eabi-nm", str(ELF)], text=True)
    symbols: dict[str, int] = {}
    for line in output.splitlines():
        parts = line.split()
        if len(parts) >= 3:
            symbols[parts[2]] = int(parts[0], 16)
    return symbols


def read_word(data: bytes, off: int) -> int:
    return struct.unpack_from("<I", data, off)[0]


def assert_veneer(data: bytes, off: int, target: int, label: str) -> None:
    got = data[off : off + 4]
    ptr = read_word(data, off + 4)
    if got != VENEER_HEAD or ptr != (target | 1):
        raise AssertionError(
            f"{label}: bad veneer at 0x{off:06X}: {got.hex()} -> 0x{ptr:08X}, "
            f"expected {VENEER_HEAD.hex()} -> 0x{target | 1:08X}"
        )
    print(f"PASS {label}: veneer at 0x{off:06X} -> 0x{ptr:08X}")


def assert_word_ref(data: bytes, symbols: dict[str, int], func: str, ref: str) -> None:
    start = symbols[func] - 0x08000000
    blob = data[start : start + 0x120]
    needle = struct.pack("<I", symbols[ref])
    if needle not in blob:
        raise AssertionError(f"{func}: missing literal ref to {ref} (0x{symbols[ref]:08X})")
    print(f"PASS {func}: references {ref} at 0x{symbols[ref]:08X}")


def assert_thumb_word_ref(data: bytes, symbols: dict[str, int], func: str, ref: str) -> None:
    start = symbols[func] - 0x08000000
    blob = data[start : start + 0x120]
    needle = struct.pack("<I", symbols[ref] | 1)
    if needle not in blob:
        raise AssertionError(
            f"{func}: missing Thumb literal ref to {ref} (0x{symbols[ref] | 1:08X})"
        )
    print(f"PASS {func}: references {ref}|1 at 0x{symbols[ref] | 1:08X}")


def main() -> int:
    if not ROM.exists() or not ELF.exists():
        raise SystemExit("Build first: make -j$(nproc)")

    data = ROM.read_bytes()
    symbols = load_symbols()

    required = [
        "PlayerDeathFx__Replacement",
        "DeleteActor__Replacement",
        "DoPhoenixRevive",
        "PhoenixDeathFxShouldSkip",
        "DeleteActor__Continue",
        "gPhoenixReviveUsed",
        "gPhoenixReviveMagicA",
        "gPhoenixReviveMagicB",
        "gPhoenixReviveTailSkips",
    ]
    missing = [name for name in required if name not in symbols]
    if missing:
        raise AssertionError(f"missing symbols: {', '.join(missing)}")

    assert_veneer(data, DEATH_FX_OFF, symbols["PlayerDeathFx__Replacement"], "PlayerDeathFx")
    # DeleteActor may be veneered for overworld fauna EXP, but must always
    # Continue (never skip) — skipping softlocks overworld transitions.
    baserom = (ROOT / "baserom.gba").read_bytes()
    delete_bytes = data[DELETE_ACTOR_OFF : DELETE_ACTOR_OFF + 8]
    if delete_bytes == baserom[DELETE_ACTOR_OFF : DELETE_ACTOR_OFF + 8]:
        print(f"PASS DeleteActor: vanilla prologue at 0x{DELETE_ACTOR_OFF:06X}")
    else:
        assert_veneer(
            data,
            DELETE_ACTOR_OFF,
            symbols["DeleteActor__Replacement"],
            "DeleteActor (overworld EXP)",
        )
        # Replacement must call Continue (relative BL is fine — no literal pool).
        repl = symbols["DeleteActor__Replacement"] - 0x08000000
        cont = symbols["DeleteActor__Continue"] - 0x08000000
        blob = data[repl : repl + 0x40]
        # Thumb BL encoding to Continue within the replacement body.
        found_bl = False
        for i in range(0, len(blob) - 3, 2):
            hw1 = struct.unpack_from("<H", blob, i)[0]
            hw2 = struct.unpack_from("<H", blob, i + 2)[0]
            if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xF800) != 0xF800:
                continue
            imm = ((hw1 & 0x7FF) << 12) | ((hw2 & 0x7FF) << 1)
            if imm & (1 << 22):
                imm -= 1 << 23
            if (repl + i + 4 + imm) & ~1 == cont:
                found_bl = True
                break
        if not found_bl:
            raise AssertionError(
                "DeleteActor__Replacement does not BL DeleteActor__Continue"
            )
        print("PASS DeleteActor__Replacement: BL DeleteActor__Continue")

    assert_word_ref(data, symbols, "DoPhoenixRevive", "gPhoenixReviveTailSkips")
    assert_word_ref(data, symbols, "PhoenixIsEquipped", "gGunLoadoutImpact")
    assert_thumb_word_ref(data, symbols, "PlayerDeathFx__Replacement", "PhoenixDeathFxSkipReturn")

    skip_start = symbols["PhoenixDeathFxSkipReturn"] - 0x08000000
    skip_blob = data[skip_start : skip_start + 0x400]
    for caller_lr, skip_to in DEATH_SKIP_RETURNS:
        if struct.pack("<I", caller_lr) not in skip_blob:
            raise AssertionError(f"missing death caller LR 0x{caller_lr:08X}")
        if struct.pack("<I", skip_to) not in skip_blob:
            raise AssertionError(f"missing death caller skip target 0x{skip_to:08X}")
    print(f"PASS PhoenixDeathFxSkipReturn: {len(DEATH_SKIP_RETURNS)} caller tails mapped")

    ram = [
        symbols["gPhoenixReviveUsed"],
        symbols["gPhoenixReviveMagicA"],
        symbols["gPhoenixReviveMagicB"],
        symbols["gPhoenixReviveTailSkips"],
    ]
    if ram != list(range(ram[0], ram[0] + len(ram))):
        raise AssertionError(f"Phoenix RAM flags are not contiguous: {[hex(x) for x in ram]}")
    if not all(0x03000000 <= addr < IWRAM_STACK_FLOOR for addr in ram):
        raise AssertionError(f"Phoenix RAM flags outside safe IWRAM: {[hex(x) for x in ram]}")
    print(f"PASS Phoenix RAM flags: {[hex(x) for x in ram]}")

    print("PASS Phoenix static verification complete")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as err:
        print(f"FAIL {err}", file=sys.stderr)
        raise SystemExit(1)
