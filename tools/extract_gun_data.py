#!/usr/bin/env python3
"""Extract Sigma Star Saga status-screen Gun Data icons to PNG.

Cannon / Bullet / Impact icons live in Shooter archive ANM #231 (1-based;
file table at 0x6188C4). Palette comes from Shooter SCN #193 sprite bank 0.

Each piece has a paired frame: even = locked ("?"), odd = owned silhouette.
This tool writes the owned (odd) icons only.

Requires: Pillow, baserom.gba in the repo root.
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    sys.exit("Pillow is required: pip install Pillow")

from extract_character_models import extract_anm_frames, write_sheet

REPO = Path(__file__).resolve().parents[1]
SHOOTER_FT = 0x6188C4
GUN_ICON_ANM = 230  # 0-based; RTB splitter names this 231.anm
SCN_PALETTE_INDEX = 192  # 0-based; 193.scn

# Description tables (fixed-width records) in ROM.
CANNON_DESC = (0x781E4, 0x50, 28)
BULLET_DESC = (0x78AA4, 0x46, 20)
IMPACT_DESC = (0x7901C, 0x5A, 28)

# Frame index formula from 0x08039F30:
#   cannon local i -> 2*i
#   bullet local i -> 2*(i + 28)
#   impact local i -> 2*(i + 48)
# Owned icons use the odd frame (+1).
TYPE_FRAME_BASE = {
    "cannon": 0,
    "bullet": 28,
    "impact": 48,
}


def shooter_file_range(rom: bytes, index: int) -> tuple[int, int]:
    count = struct.unpack_from("<I", rom, SHOOTER_FT)[0] - 1
    if not 0 <= index < count:
        raise IndexError(f"Shooter file index {index} out of range (0..{count - 1})")
    start = SHOOTER_FT + struct.unpack_from("<I", rom, SHOOTER_FT + 4 + index * 4)[0]
    end = SHOOTER_FT + struct.unpack_from("<I", rom, SHOOTER_FT + 4 + (index + 1) * 4)[0]
    return start, end


def load_scn_sprite_palette(rom: bytes, scn_start: int) -> list[int]:
    off = scn_start + 0x200
    pal: list[int] = []
    for _ in range(256):
        pb = struct.unpack_from("<H", rom, off)[0]
        off += 2
        pal.extend([(pb & 0x1F) * 8, ((pb >> 5) & 0x1F) * 8, ((pb >> 10) & 0x1F) * 8])
    return pal


def short_name(rom: bytes, addr: int) -> str:
    end = rom.find(b" :", addr)
    if end < 0:
        end = rom.find(b"\0", addr)
    raw = rom[addr:end].decode("ascii", errors="replace")
    return (
        raw.strip()
        .lower()
        .replace(" ", "_")
        .replace("/", "_")
        .replace("-", "_")
        .replace("+", "plus")
    )


def load_names(rom: bytes, base: int, stride: int, count: int) -> list[str]:
    return [short_name(rom, base + i * stride) for i in range(count)]


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--rom",
        type=Path,
        default=REPO / "baserom.gba",
        help="Path to baserom.gba",
    )
    parser.add_argument(
        "--out",
        type=Path,
        default=REPO / "graphics" / "gun_data",
        help="Output directory (cannon/bullet/impact subfolders)",
    )
    parser.add_argument(
        "--no-sheet",
        action="store_true",
        help="Skip writing per-type _sheet.png",
    )
    parser.add_argument(
        "--include-locked",
        action="store_true",
        help="Also write locked '?' variants as *_locked.png",
    )
    args = parser.parse_args()

    if not args.rom.is_file():
        sys.exit(f"ROM not found: {args.rom}")

    rom = args.rom.read_bytes()
    scn_start, _ = shooter_file_range(rom, SCN_PALETTE_INDEX)
    palette = load_scn_sprite_palette(rom, scn_start)

    anm_start, anm_end = shooter_file_range(rom, GUN_ICON_ANM)
    # Gun icons use 4bpp palette bank 0 (cyan/red/yellow status-screen colors).
    # extract_anm_frames defaults to bank 1 (character models).
    frames = extract_anm_frames(
        rom[anm_start:anm_end], palette, palette_num=0, canvas=48
    )
    if len(frames) < 152:
        sys.exit(f"Expected >= 152 gun-icon frames, got {len(frames)}")

    groups = {
        "cannon": load_names(rom, *CANNON_DESC),
        "bullet": load_names(rom, *BULLET_DESC),
        "impact": load_names(rom, *IMPACT_DESC),
    }

    total = 0
    for kind, names in groups.items():
        out_dir = args.out / kind
        out_dir.mkdir(parents=True, exist_ok=True)
        for old in out_dir.glob("*.png"):
            old.unlink()

        owned_frames: list[Image.Image] = []
        base = TYPE_FRAME_BASE[kind]
        for i, name in enumerate(names):
            locked_i = 2 * (base + i)
            owned_i = locked_i + 1
            owned = frames[owned_i]
            fname = f"{i + 1:02d}_{name}.png"
            owned.save(out_dir / fname)
            owned_frames.append(owned)
            print(f"{kind}/{fname} ({owned.width}x{owned.height})")
            if args.include_locked:
                locked = frames[locked_i]
                locked.save(out_dir / f"{i + 1:02d}_{name}_locked.png")
            total += 1

        if not args.no_sheet:
            cols = 7 if kind != "bullet" else 5
            write_sheet(owned_frames, out_dir / "_sheet.png", cols=cols)
            print(f"{kind}/_sheet.png")

    print(f"Wrote {total} gun data icons to {args.out}")


if __name__ == "__main__":
    main()
