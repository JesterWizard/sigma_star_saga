#!/usr/bin/env python3
"""Apply LynJump.event stubs and runtime-gated ROM patches."""

import pathlib
import re
import struct
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
ORG_RE = re.compile(r"ORG\s+\$([0-9A-Fa-f]+)")
WORD_RE = re.compile(r"WORD\s+(.+)")
POIN_RE = re.compile(r"POIN\s+(\w+)")
BOOL_RE = re.compile(
    r"\.(skip_flight_battle|always_run|always_max_health|always_max_bombs|"
    r"all_cannon_data|all_bullet_data|all_impact_data|"
    r"all_key_items|all_overworld_items)\s*=\s*(TRUE|FALSE|true|false|1|0)",
    re.IGNORECASE,
)

SHOOTER_CHEAT_FLAGS = (
    "always_max_health",
    "always_max_bombs",
    "all_cannon_data",
    "all_bullet_data",
    "all_impact_data",
    "all_key_items",
    "all_overworld_items",
)

FLIGHT_SKIP_OFF = 0x1749C
FLIGHT_SKIP_LEN = 0x5E

# Shooter per-frame wrapper @ 0x14E70 (14 bytes): long-jump veneer when cheats on.
SHOOTER_FRAME_OFF = 0x14E70
SHOOTER_FRAME_LEN = 0x0E
# Thumb: ldr r3, [pc, #0]; bx r3; .word hook|1
SHOOTER_FRAME_VENEER_HEAD = struct.pack("<HH", 0x4B00, 0x4718)

# Overworld B-to-run: `ands r0, r1` after `movs r0, #2` — replace with `movs r0, #2`
# so the "B held" branch always wins (doubles move speed at player+0x48/+0x54).
ALWAYS_RUN_ANDS_SITES = [
    0x1DE7C,  # speed doubling
    0x1ECFE,  # run anim / state
    0x1ED36,
    0x1ED68,
]
ANDS_R0_R1 = bytes((0x08, 0x40))  # 0x4008
MOVS_R0_2 = bytes((0x02, 0x20))  # 0x2002

# AddExperience @ 0xFDC4 — long-jump veneer when exp_multiplier != 1.
ADD_EXPERIENCE_OFF = 0xFDC4
ADD_EXPERIENCE_VENEER_LEN = 8
EXP_MULT_RE = re.compile(r"\.exp_multiplier\s*=\s*(\d+)")


def load_symbols(elf_path: pathlib.Path):
    output = subprocess.check_output(["arm-none-eabi-nm", str(elf_path)], text=True)
    symbols = {}
    for line in output.splitlines():
        parts = line.strip().split()
        if len(parts) != 3:
            continue
        addr, sym_type, name = parts
        value = int(addr, 16)
        if sym_type in {"T", "t"}:
            value |= 1
        symbols[name] = value
    return symbols


def checked_write(rom: bytearray, start: int, data: bytes, owners: dict, owner: str):
    end = start + len(data)
    if start < 0 or end > len(rom):
        raise ValueError(f"{owner} writes outside ROM bounds: 0x{start:X}-0x{end:X}")
    for offset in range(start, end):
        previous = owners.get(offset)
        if previous is not None and previous != owner:
            raise ValueError(
                f"{owner} overlaps byte 0x{offset:X} already written by {previous}"
            )
    rom[start:end] = data
    for offset in range(start, end):
        owners[offset] = owner


def apply_event(event_path: pathlib.Path, rom: bytearray, symbols, owners: dict):
    cursor = None
    for raw_line in event_path.read_text().splitlines():
        line = raw_line.split("//", 1)[0].strip()
        if not line or line in {"PUSH", "POP"}:
            continue

        match = ORG_RE.fullmatch(line)
        if match:
            cursor = int(match.group(1), 16)
            continue

        if line.startswith("ALIGN"):
            if cursor is None:
                raise ValueError(f"ALIGN before ORG in {event_path}")
            align = int(line.split()[1], 0)
            cursor = (cursor + (align - 1)) & ~(align - 1)
            continue

        match = WORD_RE.fullmatch(line)
        if match:
            if cursor is None:
                raise ValueError(f"WORD before ORG in {event_path}")
            for token in match.group(1).split():
                value = int(token[1:], 16) if token.startswith("$") else int(token, 0)
                checked_write(
                    rom, cursor, struct.pack("<I", value), owners, f"{event_path}:{line}"
                )
                cursor += 4
            continue

        match = POIN_RE.fullmatch(line)
        if match:
            if cursor is None:
                raise ValueError(f"POIN before ORG in {event_path}")
            name = match.group(1)
            if name not in symbols:
                raise KeyError(f"symbol {name} not found for {event_path}")
            checked_write(
                rom,
                cursor,
                struct.pack("<I", symbols[name]),
                owners,
                f"{event_path}:POIN {name}",
            )
            cursor += 4
            continue

        raise ValueError(f"unsupported line in {event_path}: {raw_line}")


def load_runtime_flags() -> dict[str, bool]:
    text = (ROOT / "configs" / "runtime.c").read_text()
    flags = {
        "skip_flight_battle": True,
        "always_run": False,
        "always_max_health": False,
        "always_max_bombs": False,
        "all_cannon_data": False,
        "all_bullet_data": False,
        "all_impact_data": False,
        "all_key_items": False,
        "all_overworld_items": False,
    }
    for match in BOOL_RE.finditer(text):
        name = match.group(1)
        flags[name] = match.group(2).upper() in {"TRUE", "1"}
    return flags


def load_exp_multiplier() -> int:
    text = (ROOT / "configs" / "runtime.c").read_text()
    match = EXP_MULT_RE.search(text)
    return int(match.group(1)) if match else 1


def apply_flight_skip_gate(rom: bytearray, owners: dict, enabled: bool):
    if enabled:
        print("runtime: skip_flight_battle=TRUE (in-place hook kept)")
        return
    baserom = (ROOT / "baserom.gba").read_bytes()
    checked_write(
        rom,
        FLIGHT_SKIP_OFF,
        baserom[FLIGHT_SKIP_OFF : FLIGHT_SKIP_OFF + FLIGHT_SKIP_LEN],
        owners,
        "runtime:skip_flight_battle=FALSE",
    )
    print("runtime: skip_flight_battle=FALSE (restored vanilla 0x1749C)")


def apply_always_run(rom: bytearray, owners: dict, enabled: bool):
    if not enabled:
        print("runtime: always_run=FALSE")
        return
    for off in ALWAYS_RUN_ANDS_SITES:
        cur = bytes(rom[off : off + 2])
        if cur != ANDS_R0_R1:
            raise ValueError(
                f"always_run: expected ands at 0x{off:X}, found {cur.hex()}"
            )
        checked_write(rom, off, MOVS_R0_2, owners, f"runtime:always_run@{off:X}")
    print(f"runtime: always_run=TRUE (patched {len(ALWAYS_RUN_ANDS_SITES)} B-checks)")


def apply_shooter_cheats(rom: bytearray, owners: dict, symbols: dict, flags: dict):
    baserom = (ROOT / "baserom.gba").read_bytes()
    enabled = any(flags[name] for name in SHOOTER_CHEAT_FLAGS)
    if not enabled:
        checked_write(
            rom,
            SHOOTER_FRAME_OFF,
            baserom[SHOOTER_FRAME_OFF : SHOOTER_FRAME_OFF + SHOOTER_FRAME_LEN],
            owners,
            "runtime:shooter_cheats=FALSE",
        )
        print("runtime: shooter/inventory cheats=FALSE (vanilla 0x14E70)")
        return

    name = "UpdateShooterFrame__Replacement"
    if name not in symbols:
        raise KeyError(f"symbol {name} not found (needed for shooter cheats)")
    hook = symbols[name]
    checked_write(
        rom,
        SHOOTER_FRAME_OFF,
        SHOOTER_FRAME_VENEER_HEAD + struct.pack("<I", hook),
        owners,
        "runtime:shooter_cheats",
    )
    on = [n for n in SHOOTER_CHEAT_FLAGS if flags[n]]
    print(f"runtime: shooter/inventory cheats ({', '.join(on)}) → 0x{hook:08X}")


def apply_exp_multiplier(rom: bytearray, owners: dict, symbols: dict, multiplier: int):
    baserom = (ROOT / "baserom.gba").read_bytes()
    if multiplier == 1:
        checked_write(
            rom,
            ADD_EXPERIENCE_OFF,
            baserom[ADD_EXPERIENCE_OFF : ADD_EXPERIENCE_OFF + ADD_EXPERIENCE_VENEER_LEN],
            owners,
            "runtime:exp_multiplier=1",
        )
        print("runtime: exp_multiplier=1 (vanilla AddExperience)")
        return

    name = "AddExperience__Replacement"
    if name not in symbols:
        raise KeyError(f"symbol {name} not found (needed for exp_multiplier)")
    hook = symbols[name]
    checked_write(
        rom,
        ADD_EXPERIENCE_OFF,
        SHOOTER_FRAME_VENEER_HEAD + struct.pack("<I", hook),
        owners,
        "runtime:exp_multiplier",
    )
    print(f"runtime: exp_multiplier={multiplier} → 0x{hook:08X}")


def main():
    if len(sys.argv) != 3:
        print(f"usage: {sys.argv[0]} <elf> <rom>", file=sys.stderr)
        return 1

    elf_path = pathlib.Path(sys.argv[1])
    rom_path = pathlib.Path(sys.argv[2])
    event_path = ROOT / "src_custom" / "LynJump.event"

    rom = bytearray(rom_path.read_bytes())
    symbols = load_symbols(elf_path)
    owners: dict[int, str] = {}
    flags = load_runtime_flags()
    exp_multiplier = load_exp_multiplier()

    if event_path.exists():
        apply_event(event_path, rom, symbols, owners)

    apply_flight_skip_gate(rom, owners, flags["skip_flight_battle"])
    apply_always_run(rom, owners, flags["always_run"])
    apply_shooter_cheats(rom, owners, symbols, flags)
    apply_exp_multiplier(rom, owners, symbols, exp_multiplier)

    rom_path.write_bytes(rom)
    print(f"patches: {len(owners)} bytes written")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
