#!/usr/bin/env python3
"""Extract Sigma Star Saga status-screen inventory item icons to PNG.

Icons live in Shooter archive ANM #231 (same sheet as Gun Data), frames
152–195 (type index 3 in 0x08039F30: frame = 2*(local + 76), odd = owned).
Palette comes from Shooter SCN #193 sprite bank 0.

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
ICON_ANM = 230  # 0-based; 231.anm
SCN_PALETTE_INDEX = 192  # 0-based; 193.scn

# Fixed-width description records (stride 0x64 from 0x0803A388).
ITEM_DESC_BASE = 0x799F4
ITEM_DESC_STRIDE = 0x64
ITEM_COUNT = 22  # DATA CARD … HINT (slots 0–21)
ITEM_FRAME_BASE = 76  # type-3 offset in ANM 231


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
    chunk = rom[addr : addr + ITEM_DESC_STRIDE]
    end = chunk.find(b" :")
    if end < 0:
        end = chunk.find(b":")
    if end < 0:
        end = chunk.find(b"\0")
    if end < 0:
        end = len(chunk)
    raw = chunk[:end].decode("ascii", errors="replace").strip()
    return (
        raw.lower()
        .replace(" ", "_")
        .replace("/", "_")
        .replace("-", "_")
        .replace("+", "plus")
    )


def load_item_names(rom: bytes) -> list[str]:
    """Unique filenames; repeated FLARE/FOSSIL slots get a 1-based suffix."""
    raw = [
        short_name(rom, ITEM_DESC_BASE + i * ITEM_DESC_STRIDE)
        for i in range(ITEM_COUNT)
    ]
    counts: dict[str, int] = {}
    for name in raw:
        counts[name] = counts.get(name, 0) + 1

    seen: dict[str, int] = {}
    out: list[str] = []
    for name in raw:
        if counts[name] == 1:
            out.append(name)
            continue
        seen[name] = seen.get(name, 0) + 1
        out.append(f"{name}_{seen[name]:02d}")
    return out


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
        default=REPO / "graphics" / "inventory",
        help="Output directory for PNGs",
    )
    parser.add_argument(
        "--no-sheet",
        action="store_true",
        help="Skip writing _sheet.png",
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

    anm_start, anm_end = shooter_file_range(rom, ICON_ANM)
    frames = extract_anm_frames(
        rom[anm_start:anm_end], palette, palette_num=0, canvas=48
    )
    need = 2 * (ITEM_FRAME_BASE + ITEM_COUNT)
    if len(frames) < need:
        sys.exit(f"Expected >= {need} icon frames, got {len(frames)}")

    names = load_item_names(rom)
    args.out.mkdir(parents=True, exist_ok=True)
    for old in args.out.glob("*.png"):
        old.unlink()

    owned_frames: list[Image.Image] = []
    for i, name in enumerate(names):
        locked_i = 2 * (ITEM_FRAME_BASE + i)
        owned_i = locked_i + 1
        owned = frames[owned_i]
        fname = f"{i:02d}_{name}.png"
        owned.save(args.out / fname)
        owned_frames.append(owned)
        print(f"{fname} ({owned.width}x{owned.height})")
        if args.include_locked:
            frames[locked_i].save(args.out / f"{i:02d}_{name}_locked.png")

    if not args.no_sheet:
        write_sheet(owned_frames, args.out / "_sheet.png", cols=6)
        print("_sheet.png")

    print(f"Wrote {len(names)} inventory icons to {args.out}")


if __name__ == "__main__":
    main()
