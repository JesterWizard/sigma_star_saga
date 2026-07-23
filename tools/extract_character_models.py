#!/usr/bin/env python3
"""Extract Sigma Star Saga overworld character model sprites to PNG sheets.

Character bodies are WayForward ANM (format 0) entries in the RPG file table
at 0x25154C. Palette comes from the sprite half of RPG SCN file #1, bank 1
(matches the Krill cast / Psyme / Scarlet colours used in-game).

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
SCN_PALETTE_INDEX = 0  # 1.scn
DEFAULT_PALETTE_NUM = 1

# 1-based RPG ANM file numbers -> output name (best-effort ID from art).
# Unlisted humanoid ANMs are written as anm_NNN.png.
NAMED_MODELS: dict[int, str] = {
    # ANM #12 includes both human Recker and Krill-parasite forms.
    12: "recker",
    31: "psyme",
    32: "tierney",
    33: "scarlet",
    34: "bloss",
    35: "soldier",
    36: "zelly_2",  # alternate anim set; #46 is walk-cycle zelly
    37: "nomak",
    39: "blune",
    40: "ammer",
    41: "ek",
    42: "folly",
    43: "soldier_2",
    45: "lolly",
    46: "zelly",
    47: "overlord",
    48: "sliss",
    49: "zart",
    50: "krill_parasite",
    54: "sisters",
    # Overworld fauna / field enemies (best-effort labels).
    13: "burrowing_worm",
    51: "slug",
    61: "hennk",
    62: "shell_beetle",
    63: "tick_spider",
    64: "jumping_spider",
    65: "cephalopod",
    66: "mech_spider",
    67: "hanging_spider",
    69: "multi_eye",
    70: "zombie",
}

# Extra humanoid-sized ANMs to dump even without a name.
# (Skip UI/objects; include remaining cast-sized walkers / NPCs.)
EXTRA_ANM_FILES = [
    44,  # was mis-ID'd as bloss; still dump until identified
    98,
    99,
    100,
    101,
]


def rpg_file_range(rom: bytes, index: int) -> tuple[int, int]:
    count = struct.unpack_from("<I", rom, RPG_FT)[0] - 1
    if not 0 <= index < count:
        raise IndexError(f"RPG file index {index} out of range (0..{count - 1})")
    start = RPG_FT + struct.unpack_from("<I", rom, RPG_FT + 4 + index * 4)[0]
    end = RPG_FT + struct.unpack_from("<I", rom, RPG_FT + 4 + (index + 1) * 4)[0]
    return start, end


def load_scn_sprite_palette(rom: bytes, scn_start: int, *, raw: bool = False) -> list[int]:
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
    palette_num: int = 1,
    canvas: int = 256,
) -> list[Image.Image]:
    """Assemble ANM frames and trim to opaque content (piece extents)."""
    flags, _maxpieces, _maxbytes, total, tilestart, _tilesize = struct.unpack_from(
        "<HHHHII", data, 0
    )
    frames: list[Image.Image] = []
    r0, g0, b0 = palette[0], palette[1], palette[2]

    for x in range(total):
        fo = struct.unpack_from("<I", data, 16 + x * 12)[0]
        fs = struct.unpack_from("<I", data, 16 + x * 12 + 4)[0] + tilestart
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
            pn = palette_num + (1 if (pf & 0x4000) else 0)
            tile_img = Image.new("P", (tw * 8, th * 8), 0)
            tile_img.putpalette(palette)
            off = fs + (pf & 0x03FF) * 32
            tpx = tpy = 0
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
                        a = (tb & 0x0F) + (pn * 16)
                        b = ((tb & 0xF0) >> 4) + (pn * 16)
                        if a & 0x0F == 0:
                            a = 0
                        if b & 0x0F == 0:
                            b = 0
                        temp.append(a)
                        temp.append(b)
                    crop = Image.frombuffer("L", (8, 8), bytes(temp), "raw", "L", 0, 1)
                tile_img.paste(crop, (tpx * 8, tpy * 8))
                tpx += 1
                if tpx == tw:
                    tpx = 0
                    tpy += 1
            img.paste(tile_img, (pivot_x[s] + canvas // 2, pivot_y[s] + canvas // 2))

        rgba = img.convert("RGBA")
        pixels = rgba.load()
        for yy in range(canvas):
            for xx in range(canvas):
                r, g, b, _a = pixels[xx, yy]
                if (r, g, b) == (r0, g0, b0):
                    pixels[xx, yy] = (0, 0, 0, 0)
        bbox = rgba.getbbox()
        frames.append(rgba.crop(bbox) if bbox else rgba)
    return frames


def write_sheet(frames: list[Image.Image], path: Path, cols: int | None = None) -> None:
    if cols is None:
        cols = 16 if len(frames) > 24 else 8
    fw = max(f.width for f in frames)
    fh = max(f.height for f in frames)
    rows = (len(frames) + cols - 1) // cols
    sheet = Image.new("RGBA", (cols * fw, rows * fh), (0, 0, 0, 0))
    for i, fr in enumerate(frames):
        x = (i % cols) * fw + (fw - fr.width) // 2
        y = (i // cols) * fh + (fh - fr.height) // 2
        sheet.paste(fr, (x, y), fr)
    sheet.save(path)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=REPO / "baserom.gba")
    parser.add_argument(
        "--out", type=Path, default=REPO / "graphics" / "character_models"
    )
    parser.add_argument(
        "--frames",
        action="store_true",
        help="Also write per-character frame folders (000.png, ...)",
    )
    parser.add_argument(
        "--palette-num",
        type=int,
        default=DEFAULT_PALETTE_NUM,
        help="4bpp palette bank (0-15), default 1",
    )
    args = parser.parse_args()

    if not args.rom.is_file():
        sys.exit(f"ROM not found: {args.rom}")

    rom = args.rom.read_bytes()
    scn_start, _ = rpg_file_range(rom, SCN_PALETTE_INDEX)
    palette = load_scn_sprite_palette(rom, scn_start)

    jobs: list[tuple[int, str]] = []
    for file_num, name in sorted(NAMED_MODELS.items()):
        jobs.append((file_num, name))
    for file_num in EXTRA_ANM_FILES:
        if file_num not in NAMED_MODELS:
            jobs.append((file_num, f"anm_{file_num:03d}"))

    args.out.mkdir(parents=True, exist_ok=True)
    for old in args.out.glob("*.png"):
        old.unlink()

    for file_num, name in jobs:
        # RTB / BMS file numbers are 1-based; table index is 0-based.
        start, end = rpg_file_range(rom, file_num - 1)
        frames = extract_anm_frames(
            rom[start:end], palette, palette_num=args.palette_num
        )
        sheet_path = args.out / f"{name}.png"
        write_sheet(frames, sheet_path)
        print(f"{name}.png  (ANM #{file_num}, {len(frames)} frames, cell~{frames[0].size})")

        if args.frames:
            frame_dir = args.out / name
            if frame_dir.exists():
                for old in frame_dir.glob("*.png"):
                    old.unlink()
            frame_dir.mkdir(parents=True, exist_ok=True)
            for i, fr in enumerate(frames):
                fr.save(frame_dir / f"{i:03d}.png")

    print(f"Wrote {len(jobs)} character model sheets to {args.out}")


if __name__ == "__main__":
    main()
