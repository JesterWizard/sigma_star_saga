#!/usr/bin/env python3
"""Extract Sigma Star Saga dialogue/menu character portraits to PNG.

Portraits live in RPG archive ANM file #19 (1-based; file table at 0x25154C).
Palette comes from the sprite half of RPG SCN file #1.

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

REPO = Path(__file__).resolve().parents[1]
RPG_FT = 0x25154C
PORTRAIT_FILE_INDEX = 18  # 0-based; RTB splitter names this 19.anm
SCN_PALETTE_INDEX = 0  # 1.scn

# Frame order matches the in-game portrait ANM (includes nameplates).
PORTRAIT_NAMES = [
    "recker",
    "recker_krill",
    "psyme",
    "scarlet",
    "tierney",
    "blune",
    "soldier",
    "medic",
    "bloss",
    "zelly",
    "nomak",
    "zart",
    "ammer",
    "folly",
    "lolly",
    "sliss",
    "sisters",
    "iot",
    "ek",
    "crewman",
    "tech",
    "overlord",
    "unknown",
    "medic_1",
    "medic_2",
    "medic_3",
    "grunt_1",
    "grunt_2",
    "overlord_shadow",
]


def rpg_file_range(rom: bytes, index: int) -> tuple[int, int]:
    count = struct.unpack_from("<I", rom, RPG_FT)[0] - 1
    if not 0 <= index < count:
        raise IndexError(f"RPG file index {index} out of range (0..{count - 1})")
    start = RPG_FT + struct.unpack_from("<I", rom, RPG_FT + 4 + index * 4)[0]
    end = RPG_FT + struct.unpack_from("<I", rom, RPG_FT + 4 + (index + 1) * 4)[0]
    return start, end


def load_scn_sprite_palette(rom: bytes, scn_start: int, *, raw: bool = True) -> list[int]:
    # SCN: 0x200 BG palettes, then 0x200 sprite palettes (256 × RGB555).
    off = scn_start + 0x200
    pal: list[int] = []
    for _ in range(256):
        pb = struct.unpack_from("<H", rom, off)[0]
        off += 2
        r = (pb & 0x1F) * 8
        g = ((pb >> 5) & 0x1F) * 8
        b = ((pb >> 10) & 0x1F) * 8
        if not raw:
            r += (r + 1) // 32
            g += (g + 1) // 32
            b += (b + 1) // 32
        pal.extend([r, g, b])
    return pal


def piece_dims(piece_size: int) -> tuple[int, int] | None:
    return {
        0x0000: (1, 1),
        0x0400: (2, 1),
        0x0800: (1, 2),
        0x1000: (2, 2),
        0x1400: (4, 1),
        0x1800: (1, 4),
        0x2000: (4, 4),
        0x2400: (4, 2),
        0x2800: (2, 4),
        0x3000: (8, 8),
        0x3400: (8, 4),
        0x3800: (4, 8),
    }.get(piece_size & 0x3C00)


def extract_anm_frames(
    data: bytes,
    palette: list[int],
    *,
    palette_num: int = 0,
    canvas: int = 256,
) -> list[Image.Image]:
    """WayForward ANM format 0 (Sigma Star Saga), matching RTB's extractor."""
    flags, _maxpieces, _maxbytes, total, tilestart, _tilesize = struct.unpack_from(
        "<HHHHII", data, 0
    )
    frames: list[Image.Image] = []
    for x in range(total):
        fo = struct.unpack_from("<I", data, 16 + x * 12)[0]
        fs = struct.unpack_from("<I", data, 16 + x * 12 + 4)[0] + tilestart
        vals = struct.unpack_from("<" + "h" * 12, data, fo)
        x1, x2, y1, y2 = vals[0], vals[3], vals[6], vals[9]
        piece_count = struct.unpack_from("<H", data, fo + 24)[0]
        pos = fo + 26
        pivot_x = struct.unpack_from("<" + "h" * piece_count, data, pos)
        pos += 2 * piece_count
        pivot_y = struct.unpack_from("<" + "h" * piece_count, data, pos)
        pos += 2 * piece_count
        piece_flags = struct.unpack_from("<" + "H" * piece_count, data, pos)

        img = Image.new("P", (canvas, canvas), 0)
        img.putpalette(palette)
        for s in range(piece_count):
            pf = piece_flags[s]
            dims = piece_dims(pf)
            if not dims:
                continue
            tw, th = dims
            tile_img = Image.new("P", (tw * 8, th * 8), 0)
            tile_img.putpalette(palette)
            off = fs + (pf & 0x03FF) * 32
            px = py = 0
            for _t in range(tw * th):
                if (pf & 0x8000) or flags == 0x8000:
                    chunk = data[off : off + 0x40]
                    off += 0x40
                    crop = Image.frombuffer("L", (8, 8), chunk, "raw", "L", 0, 1)
                else:
                    temp = bytearray()
                    for _y in range(32):
                        tb = data[off]
                        off += 1
                        a = (tb & 0x0F) + (palette_num * 16)
                        b = ((tb & 0xF0) >> 4) + (palette_num * 16)
                        if a & 0x0F == 0:
                            a = 0
                        if b & 0x0F == 0:
                            b = 0
                        temp.append(a)
                        temp.append(b)
                    crop = Image.frombuffer("L", (8, 8), bytes(temp), "raw", "L", 0, 1)
                tile_img.paste(crop, (px * 8, py * 8))
                px += 1
                if px == tw:
                    px = 0
                    py += 1
            img.paste(tile_img, (pivot_x[s] + canvas // 2, pivot_y[s] + canvas // 2))

        left = max(0, canvas // 2 + x1)
        top = max(0, canvas // 2 + y1)
        right = min(canvas, canvas // 2 + x2 + 1)
        bottom = min(canvas, canvas // 2 + y2 + 1)
        cropped = img.crop((left, top, right, bottom)).convert("RGBA")
        r0, g0, b0 = palette[0], palette[1], palette[2]
        pixels = cropped.load()
        for yy in range(cropped.height):
            for xx in range(cropped.width):
                r, g, b, _a = pixels[xx, yy]
                if (r, g, b) == (r0, g0, b0):
                    pixels[xx, yy] = (0, 0, 0, 0)
        frames.append(cropped)
    return frames


def write_sheet(frames: list[Image.Image], path: Path, cols: int = 8) -> None:
    fw = max(f.width for f in frames)
    fh = max(f.height for f in frames)
    rows = (len(frames) + cols - 1) // cols
    sheet = Image.new("RGBA", (cols * fw, rows * fh), (0, 0, 0, 0))
    for i, fr in enumerate(frames):
        sheet.paste(fr, ((i % cols) * fw, (i // cols) * fh), fr)
    sheet.save(path)


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
        default=REPO / "graphics" / "portraits",
        help="Output directory for PNGs",
    )
    parser.add_argument(
        "--no-sheet",
        action="store_true",
        help="Skip writing _sheet.png",
    )
    args = parser.parse_args()

    if not args.rom.is_file():
        sys.exit(f"ROM not found: {args.rom}")

    rom = args.rom.read_bytes()
    scn_start, _ = rpg_file_range(rom, SCN_PALETTE_INDEX)
    palette = load_scn_sprite_palette(rom, scn_start)

    anm_start, anm_end = rpg_file_range(rom, PORTRAIT_FILE_INDEX)
    frames = extract_anm_frames(rom[anm_start:anm_end], palette)
    if len(frames) != len(PORTRAIT_NAMES):
        sys.exit(
            f"Expected {len(PORTRAIT_NAMES)} portrait frames, got {len(frames)}"
        )

    args.out.mkdir(parents=True, exist_ok=True)
    for old in args.out.glob("*.png"):
        old.unlink()

    for name, fr in zip(PORTRAIT_NAMES, frames):
        fr.save(args.out / f"{name}.png")
        print(f"{name}.png ({fr.width}x{fr.height})")

    if not args.no_sheet:
        write_sheet(frames, args.out / "_sheet.png")
        print("_sheet.png")

    print(f"Wrote {len(frames)} portraits to {args.out}")


if __name__ == "__main__":
    main()
