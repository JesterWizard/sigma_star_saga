#!/usr/bin/env python3
"""Scan baserom Thumb LDR literal pools for IWRAM / EWRAM / SRAM-bus addresses.

Prefer pool-backed hits over raw absolute word matches (graphics/data false positives).

Usage:
  python3 tools/scan_ram_literals.py [--rom baserom.gba] [--emit-asm]
"""

from __future__ import annotations

import argparse
import struct
import sys
from collections import defaultdict
from pathlib import Path

ROM_BASE = 0x08000000

# Known symbols (address -> name). Keep in sync with asm/ram_map_*.s named entries.
KNOWN_IWRAM = {
    0x03000DB8: "gPlayerPtr",
    0x03001630: "gMode",
    0x03007014: "gPlayerBombs",
    0x0300702C: "gPlayerLevel",
    0x0300705C: "gPlayerExp",
    0x03007080: "gEnemyRemaining",
    0x0300717C: "gExpToCurrentLevel",
    0x030071F0: "gExpToNextLevel",
    0x03007668: "gStageClearFlag",
    0x03007690: "gGunDataBits",
    0x03007730: "gStageClearGate",
    0x0300773C: "gBulletOwned",
    0x03007740: "gItemsOwned",
    0x03007744: "gCannonOwned",
    0x03007748: "gImpactOwned",
    0x03007CA0: "gUserStackTop",
    0x03007FA0: "gIrqStackTop",
}


def in_iwram(a: int) -> bool:
    return 0x03000000 <= a <= 0x03007FFF


def in_ewram(a: int) -> bool:
    return 0x02000000 <= a <= 0x0203FFFF


def in_sram(a: int) -> bool:
    return 0x0E000000 <= a <= 0x0E00FFFF


def scan_pools(rom: bytes) -> dict[str, dict[int, list[int]]]:
    """Return region -> {address: [ldr_file_offsets]}."""
    hits: dict[str, dict[int, list[int]]] = {
        "iwram": defaultdict(list),
        "ewram": defaultdict(list),
        "sram": defaultdict(list),
    }
    size = len(rom)
    for off in range(0, size - 2, 2):
        hw = rom[off] | (rom[off + 1] << 8)
        if (hw & 0xF800) != 0x4800:
            continue
        imm = hw & 0xFF
        pc = (ROM_BASE + off + 4) & ~2
        pool_off = pc + imm * 4 - ROM_BASE
        if pool_off < 0 or pool_off + 4 > size:
            continue
        word = struct.unpack_from("<I", rom, pool_off)[0]
        if in_iwram(word):
            hits["iwram"][word].append(off)
        elif in_ewram(word):
            hits["ewram"][word].append(off)
        elif in_sram(word):
            hits["sram"][word].append(off)
    return hits


def clusters(addrs: list[int], merge_gap: int) -> list[tuple[int, int]]:
    if not addrs:
        return []
    out: list[tuple[int, int]] = []
    start = end = addrs[0]
    for a in addrs[1:]:
        if a - end <= merge_gap:
            end = a
        else:
            out.append((start, end))
            start = end = a
    out.append((start, end))
    return out


def symbol_name(addr: int, known: dict[int, str]) -> str:
    if addr in known:
        return known[addr]
    return f"gUnk_{addr:08X}"


def emit_iwram_asm(hits: dict[int, list[int]], out: Path) -> None:
    # Unknowns only — named symbols live in ram_map_iwram.s.
    aligned = sorted(
        a
        for a in hits
        if a % 4 == 0 and a < 0x03007CA0 and a not in KNOWN_IWRAM
    )
    lines: list[str] = []
    lines.append("@ =============================================================================")
    lines.append("@ AUTO-GENERATED — do not edit by hand.")
    lines.append("@   python3 tools/scan_ram_literals.py --emit-asm")
    lines.append("@ Word-aligned Thumb LDR literal-pool IWRAM targets below user stack,")
    lines.append("@ excluding symbols already named in ram_map_iwram.s.")
    lines.append("@ =============================================================================")
    lines.append("")
    for a in aligned:
        n = len(hits[a])
        lines.append(f"SET_DATA gUnk_{a:08X}, 0x{a:08X}  @ pool hits={n}")
    out.write_text("\n".join(lines) + "\n")
    print(f"Wrote {out} ({len(aligned)} symbols)", file=sys.stderr)


def emit_ewram_asm(hits: dict[int, list[int]], out: Path) -> None:
    # High-confidence: aligned, below provisional free floor, hits >= 2
    # (single-hit high addresses are often art/data false positives).
    aligned = sorted(
        a for a in hits if a % 4 == 0 and a < 0x02030000 and len(hits[a]) >= 2
    )
    lines: list[str] = []
    lines.append("@ =============================================================================")
    lines.append("@ AUTO-GENERATED — do not edit by hand.")
    lines.append("@   python3 tools/scan_ram_literals.py --emit-asm")
    lines.append("@ Word-aligned EWRAM pool targets with hits>=2 below FreeEwramSpaceTop.")
    lines.append("@ Single-hit / unaligned / >=0x02030000 omitted (likely false positives).")
    lines.append("@ =============================================================================")
    lines.append("")
    for a in aligned:
        n = len(hits[a])
        lines.append(f"SET_DATA gUnk_{a:08X}, 0x{a:08X}  @ pool hits={n}")
    out.write_text("\n".join(lines) + "\n")
    print(f"Wrote {out} ({len(aligned)} symbols)", file=sys.stderr)


def print_report(hits: dict[str, dict[int, list[int]]]) -> None:
    for region, merge in (("iwram", 0x40), ("ewram", 0x100), ("sram", 0x100)):
        d = hits[region]
        aligned = sorted(a for a in d if a % 4 == 0)
        print(f"\n=== {region.upper()} pool-backed aligned: {len(aligned)} ===")
        if not aligned:
            continue
        print(f"range 0x{min(aligned):08X}–0x{max(aligned):08X}")
        freq = sorted(((a, len(v)) for a, v in d.items() if a % 4 == 0), key=lambda x: -x[1])
        print("top 20:")
        for a, c in freq[:20]:
            name = KNOWN_IWRAM.get(a, "")
            print(f"  0x{a:08X}  hits={c:4d}  {name}")
        print(f"clusters (merge gap < {merge:#x}):")
        for s, e in clusters(aligned, merge):
            n = sum(1 for a in aligned if s <= a <= e)
            h = sum(len(d[a]) for a in aligned if s <= a <= e)
            print(f"  0x{s:08X}–0x{e:08X}  unique={n:3d}  hits={h:5d}  span={e - s + 4:#x}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--rom", type=Path, default=Path("baserom.gba"))
    ap.add_argument(
        "--emit-asm",
        action="store_true",
        help="Write inventory fragments under asm/ (included by ram_map_*.s)",
    )
    args = ap.parse_args()
    if not args.rom.is_file():
        print(f"missing ROM: {args.rom}", file=sys.stderr)
        return 1
    rom = args.rom.read_bytes()
    hits = scan_pools(rom)
    print_report(hits)
    if args.emit_asm:
        out_dir = Path("asm")
        out_dir.mkdir(parents=True, exist_ok=True)
        emit_iwram_asm(hits["iwram"], out_dir / "ram_map_iwram_pool.inc")
        emit_ewram_asm(hits["ewram"], out_dir / "ram_map_ewram_pool.inc")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
