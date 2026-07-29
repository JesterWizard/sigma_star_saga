#!/usr/bin/env python3
"""Extract Sigma Star Saga full-screen CG / background posters to PNG.

CGs are WayForward TS8 + LYR pairs in the RPG file table at 0x25154C.
Tile graphics and LYR screen maps are LZ77-compressed in-ROM (same layout
the RTB QuickBMS splitter decompresses). Palettes come from the matching
SCN file in each CG cluster.

Default export covers Scarlet ("Charlet"), Psyme, Overlord / final-battle,
and related ending / key-art posters. Pass --all for remaining unnamed
8bpp CGs (space BGs, title, ship art, etc.).

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
GBA_W, GBA_H = 240, 160

# (name, ts 1-based, lyr 1-based, scn 1-based)
# Scarlet Keys is the in-game name; "Charlet" is a common misspelling.
NAMED_CGS: list[tuple[str, int, int, int]] = [
    # Cluster A — Vuduberi key art (SCN 286–288)
    ("group_keyart", 280, 281, 286),
    ("psyme", 282, 283, 287),
    ("scarlet", 284, 285, 288),
    # Mid-story banners (SCN 244–246)
    ("scarlet_banner", 234, 235, 244),
    ("psyme_banner", 236, 237, 245),
    # Ending / chapter CGs (SCN 318–327)
    ("scarlet_pose", 298, 299, 318),
    ("psyme_held", 300, 301, 319),
    ("overlord_hand", 302, 303, 320),
    ("overlord_roar", 304, 305, 321),
    ("overlord_cosmos", 306, 307, 322),
    ("scarlet_doorway", 308, 309, 323),
    ("scarlet_dive", 310, 311, 324),
    ("overlord_final_battle", 312, 313, 325),
    ("final_ships", 314, 315, 326),
    ("final_rocket", 316, 317, 327),
]

# Other 8bpp single-screen CGs dumped only with --all.
EXTRA_CGS: list[tuple[int, int, int]] = [
    (177, 178, 179),  # title screen
    (180, 181, 191),
    (242, 243, 246),  # fighter ship
    (251, 252, 269),  # space BG
    (254, 255, 270),
    (257, 258, 271),
    (260, 261, 272),
    (263, 264, 273),
    (266, 267, 274),
]


def rpg_file_range(rom: bytes, index: int) -> tuple[int, int]:
    count = struct.unpack_from("<I", rom, RPG_FT)[0] - 1
    if not 0 <= index < count:
        raise IndexError(f"RPG file index {index} out of range (0..{count - 1})")
    start = RPG_FT + struct.unpack_from("<I", rom, RPG_FT + 4 + index * 4)[0]
    end = RPG_FT + struct.unpack_from("<I", rom, RPG_FT + 4 + (index + 1) * 4)[0]
    return start, end


def lz77_decomp(src: bytes) -> bytes:
    """GBA BIOS LZ77UnCompWram (header type 0x10)."""
    if not src or src[0] != 0x10:
        raise ValueError(f"bad LZ77 header: {src[:4].hex() if src else 'empty'}")
    size = src[1] | (src[2] << 8) | (src[3] << 16)
    i = 4
    out = bytearray()
    while len(out) < size:
        if i >= len(src):
            raise ValueError(f"truncated LZ77 (need {size}, got {len(out)})")
        flags = src[i]
        i += 1
        for bit in range(8):
            if len(out) >= size:
                break
            if flags & (0x80 >> bit):
                if i + 1 >= len(src):
                    raise ValueError("truncated LZ77 backreference")
                b1, b2 = src[i], src[i + 1]
                i += 2
                disp = (((b1 & 0xF) << 8) | b2) + 1
                length = (b1 >> 4) + 3
                for _ in range(length):
                    out.append(out[-disp])
            else:
                out.append(src[i])
                i += 1
    return bytes(out)


def decompress_ts(raw: bytes) -> bytes:
    """Unpack an in-ROM TS4/TS8 (flags 0x0004 / 0x0005 = LZ77 tile payload)."""
    flags, meta_count, tile_count, unk = struct.unpack_from("<4H", raw, 0)
    header_len = 8 + meta_count * 8
    if flags in (0x0004, 0x0005):
        tiles = lz77_decomp(raw[header_len:])
        out_flags = 0x0001 if flags == 0x0005 else 0x0000
        return (
            struct.pack("<4H", out_flags, meta_count, tile_count, unk)
            + raw[8:header_len]
            + tiles
        )
    return raw


def decompress_lyr(raw: bytes) -> bytes:
    """Unpack an in-ROM LYR (flags 0x0090 / 0x00A0 = LZ77 screen payload)."""
    flags, w, h, count, ua, ub, uc, _types, _uidb, _tsid = struct.unpack_from(
        "<10H", raw, 0
    )
    grids = w * h * 2
    pos = 20 + grids * 3 + ua * 20 + ub * 8 + uc * 16
    screen_start = pos + (2 if pos % 4 else 0)
    header_len = screen_start - (2 if pos % 4 else 0)
    header = bytearray(raw[:header_len])
    # Match RTB splitter: rewrite compressed flag nibble after decompress.
    if flags == 0x0090:
        header[0] = 0x10
    elif flags == 0x00A0:
        header[0] = 0x20
    return bytes(header) + lz77_decomp(raw[screen_start:])


def load_scn_bg_palette(rom: bytes, scn_index0: int) -> list[int]:
    start, _ = rpg_file_range(rom, scn_index0)
    pal: list[int] = []
    for i in range(256):
        pb = struct.unpack_from("<H", rom, start + i * 2)[0]
        pal.extend([(pb & 0x1F) * 8, ((pb >> 5) & 0x1F) * 8, ((pb >> 10) & 0x1F) * 8])
    return pal


def build_metatile_sheet(data: bytes, palette: list[int]) -> Image.Image:
    flags, meta_count, _tile_count, _unk = struct.unpack_from("<4H", data, 0)
    tile_offset = 8 + meta_count * 8
    sheet_h = (meta_count & 0xFFF0) + (16 if (meta_count & 0xF) else 0)
    sheet = Image.new("P", (256, max(sheet_h, 16)), 0)
    sheet.putpalette(palette)

    mx = my = 0
    pos = 8
    is8 = bool(flags & 1)
    for _ in range(meta_count):
        for quad in range(4):
            mf = struct.unpack_from("<H", data, pos)[0]
            pos += 2
            tid = mf & 0x03FF
            flip = (mf & 0x0C00) >> 10
            tpal = (mf & 0xF000) >> 8
            if is8:
                off = tile_offset + tid * 0x40
                crop = Image.frombuffer(
                    "L", (8, 8), bytes(data[off : off + 0x40]), "raw", "L", 0, 1
                )
            else:
                off = tile_offset + tid * 0x20
                temp = bytearray()
                for i in range(32):
                    tb = data[off + i]
                    a = (tb & 0x0F) + tpal
                    b = ((tb & 0xF0) >> 4) + tpal
                    if a & 0x0F == 0:
                        a = 0
                    if b & 0x0F == 0:
                        b = 0
                    temp.append(a)
                    temp.append(b)
                crop = Image.frombuffer("L", (8, 8), bytes(temp), "raw", "L", 0, 1)
            if flip == 1:
                crop = crop.transpose(Image.FLIP_LEFT_RIGHT)
            elif flip == 2:
                crop = crop.transpose(Image.FLIP_TOP_BOTTOM)
            elif flip == 3:
                crop = crop.transpose(Image.FLIP_LEFT_RIGHT).transpose(
                    Image.FLIP_TOP_BOTTOM
                )
            ox = mx * 16 + (8 if quad in (1, 3) else 0)
            oy = my * 16 + (8 if quad in (2, 3) else 0)
            sheet.paste(crop, (ox, oy))
        mx += 1
        if mx == 16:
            mx = 0
            my += 1
    return sheet.convert("RGBA")


def render_lyr(data: bytes, metatile: Image.Image) -> Image.Image:
    flags, w, h, count, ua, ub, uc, _types, _uidb, _tsid = struct.unpack_from(
        "<10H", data, 0
    )
    pos = 20
    screen_ids = [
        struct.unpack_from("<H", data, pos + i * 2)[0] for i in range(w * h)
    ]
    pos += w * h * 2
    pos += w * h * 2 * 2
    pos += ua * 20 + ub * 8 + uc * 16
    mask = (
        0x03FF
        if flags in (0x0010, 0x0020)
        else (0x0FFF if flags == 0x0040 else 0x07FF)
    )

    screens: list[Image.Image] = []
    for _ in range(count):
        screen = Image.new("RGBA", (256, 256), (0, 0, 0, 0))
        for y in range(16):
            for x in range(16):
                mid = struct.unpack_from("<H", data, pos)[0] & mask
                pos += 2
                sx = (mid & 0x000F) * 16
                sy = mid & (
                    0x03F0
                    if mask == 0x03FF
                    else (0x0FF0 if mask == 0x0FFF else 0x07F0)
                )
                screen.paste(
                    metatile.crop((sx, sy, sx + 16, sy + 16)), (x * 16, y * 16)
                )
        screens.append(screen)

    out = Image.new("RGBA", (w * 256, h * 256), (0, 0, 0, 0))
    for i, sid in enumerate(screen_ids):
        if 0 <= sid < len(screens):
            out.paste(screens[sid], ((i % w) * 256, (i // w) * 256))
    return out


def is_pad_rgb(r: int, g: int, b: int) -> bool:
    # Unused metatile fill is neon/lime green or magic magenta (varies by SCN).
    if g >= 200 and r <= 96 and b <= 96:
        return True
    if r >= 200 and b >= 160 and g <= 64:
        return True
    return False


def content_bbox(rgb: Image.Image) -> tuple[int, int, int, int] | None:
    """Inclusive bbox of non-pad pixels, or None if empty."""
    minx, miny, maxx, maxy = rgb.width, rgb.height, -1, -1
    for y in range(rgb.height):
        for x in range(rgb.width):
            if is_pad_rgb(*rgb.getpixel((x, y))):
                continue
            minx = min(minx, x)
            miny = min(miny, y)
            maxx = max(maxx, x)
            maxy = max(maxy, y)
    if maxx < 0:
        return None
    return minx, miny, maxx, maxy


def trim_solid_margins(rgb: Image.Image, *, is_empty) -> Image.Image:
    """Shrink away rows/cols that are entirely empty (pad or solid black)."""
    minx, miny, maxx, maxy = 0, 0, rgb.width - 1, rgb.height - 1
    while miny <= maxy and all(is_empty(rgb.getpixel((x, miny))) for x in range(rgb.width)):
        miny += 1
    while maxy >= miny and all(is_empty(rgb.getpixel((x, maxy))) for x in range(rgb.width)):
        maxy -= 1
    while minx <= maxx and all(is_empty(rgb.getpixel((minx, y))) for y in range(miny, maxy + 1)):
        minx += 1
    while maxx >= minx and all(is_empty(rgb.getpixel((maxx, y))) for y in range(miny, maxy + 1)):
        maxx -= 1
    if maxx < minx or maxy < miny:
        return rgb
    return rgb.crop((minx, miny, maxx + 1, maxy + 1))


def crop_cg(img: Image.Image) -> Image.Image:
    """Trim pad colours; keep full art (including multi-screen banners)."""
    rgb = img.convert("RGB")
    box = content_bbox(rgb)
    if box is None:
        return rgb
    minx, miny, maxx, maxy = box
    cropped = rgb.crop((minx, miny, maxx + 1, maxy + 1))

    # Many CGs are authored as a 240×160 viewport at the origin of a 256×256
    # screen with pad fill. Prefer that viewport when it holds nearly all art.
    if (
        minx == 0
        and miny == 0
        and img.width >= GBA_W
        and img.height >= GBA_H
        and maxx >= GBA_W - 1
        and maxy >= GBA_H - 1
    ):
        outside = 0
        for y in range(rgb.height):
            for x in range(rgb.width):
                if x < GBA_W and y < GBA_H:
                    continue
                if not is_pad_rgb(*rgb.getpixel((x, y))):
                    outside += 1
        if outside < 64:
            return rgb.crop((0, 0, GBA_W, GBA_H))

    # Oversized canvases (2×1 banners, etc.) also have empty black screen tiles.
    if cropped.width > GBA_W or cropped.height > GBA_H:
        def empty(c: tuple[int, int, int]) -> bool:
            r, g, b = c
            return is_pad_rgb(r, g, b) or r + g + b < 12

        cropped = trim_solid_margins(cropped, is_empty=empty)
    return cropped


def extract_cg(rom: bytes, ts: int, lyr: int, scn: int) -> Image.Image:
    """ts/lyr/scn are 1-based RPG file numbers (RTB naming)."""
    palette = load_scn_bg_palette(rom, scn - 1)
    ts_data = decompress_ts(rom[slice(*rpg_file_range(rom, ts - 1))])
    lyr_data = decompress_lyr(rom[slice(*rpg_file_range(rom, lyr - 1))])
    sheet = build_metatile_sheet(ts_data, palette)
    return crop_cg(render_lyr(lyr_data, sheet))


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
        default=REPO / "graphics" / "cgs",
        help="Output directory for PNGs",
    )
    parser.add_argument(
        "--all",
        action="store_true",
        help="Also dump remaining unnamed 8bpp CGs as lyr_NNN.png",
    )
    args = parser.parse_args()

    if not args.rom.is_file():
        sys.exit(f"ROM not found: {args.rom}")

    rom = args.rom.read_bytes()
    args.out.mkdir(parents=True, exist_ok=True)
    for old in args.out.glob("*.png"):
        old.unlink()

    written = 0
    named_lyrs = {lyr for _, _, lyr, _ in NAMED_CGS}
    for name, ts, lyr, scn in NAMED_CGS:
        img = extract_cg(rom, ts, lyr, scn)
        path = args.out / f"{name}.png"
        img.save(path)
        print(f"{path.name} ({img.width}x{img.height}) ts={ts} lyr={lyr} scn={scn}")
        written += 1

    if args.all:
        for ts, lyr, scn in EXTRA_CGS:
            if lyr in named_lyrs:
                continue
            img = extract_cg(rom, ts, lyr, scn)
            path = args.out / f"lyr_{lyr:03d}.png"
            img.save(path)
            print(f"{path.name} ({img.width}x{img.height}) ts={ts} scn={scn}")
            written += 1

    print(f"Wrote {written} CGs to {args.out}")


if __name__ == "__main__":
    main()
