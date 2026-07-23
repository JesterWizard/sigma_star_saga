#!/usr/bin/env python3
"""Extract Sigma Star Saga overworld maps to PNG.

Maps are WayForward LYR layers (format 3) paired with TS4/TS8 tilesets and
SCN palettes. Compressed assets must be pre-split with RTB's QuickBMS script:

  https://github.com/RandomTBush/RTB-QuickBMS-Scripts
  Archive/WayForwardGBA_SigmaStarSaga_ROMSplitter.bms

Point --rpg-dir at the resulting SigmaStarSaga_RPG folder (decompressed
.lyr / .ts4 / .ts8 / .scn files).

Requires: Pillow.
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
DEFAULT_RPG_DIRS = [
    REPO / "data" / "SigmaStarSaga_RPG",
    Path("/tmp/sss_extract/out/SigmaStarSaga_RPG"),
]

# Skip tiny 1×1 UI / dialog layers.
MIN_SCREENS = 4


def find_rpg_dir(explicit: Path | None) -> Path:
    if explicit is not None:
        if not explicit.is_dir():
            sys.exit(f"RPG dump not found: {explicit}")
        return explicit
    for cand in DEFAULT_RPG_DIRS:
        if cand.is_dir() and any(cand.glob("*.lyr")):
            return cand
    sys.exit(
        "No SigmaStarSaga_RPG dump found. Pass --rpg-dir pointing at the "
        "QuickBMS output folder (see script docstring)."
    )


def load_scn_bg_palette(path: Path) -> list[int]:
    data = path.read_bytes()
    pal: list[int] = []
    for i in range(256):
        pb = struct.unpack_from("<H", data, i * 2)[0]
        pal.extend([(pb & 0x1F) * 8, ((pb >> 5) & 0x1F) * 8, ((pb >> 10) & 0x1F) * 8])
    return pal


def nearest_scn(rpg: Path, tileset_id: int) -> Path | None:
    scns = sorted(int(p.stem) for p in rpg.glob("*.scn"))
    below = [s for s in scns if s < tileset_id]
    if not below:
        return None
    return rpg / f"{below[-1]}.scn"


def build_metatile_sheet(ts_path: Path, palette: list[int]) -> Image.Image:
    """TSFormat = 1 (Sigma Star Saga) metatile sheet, 16 wide."""
    data = ts_path.read_bytes()
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
                    "L", (8, 8), data[off : off + 0x40], "raw", "L", 0, 1
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


def parse_lyr(data: bytes) -> tuple[int, int, int, int, int, int, int, int, list[int], int]:
    """Return flags, w, h, count, ua, ub, uc, tsid, screen_ids, screen_data_off."""
    flags, w, h, count, ua, ub, uc, _types, _uidb, tsid = struct.unpack_from(
        "<10H", data, 0
    )
    pos = 20
    screen_ids = [
        struct.unpack_from("<H", data, pos + i * 2)[0] for i in range(w * h)
    ]
    pos += w * h * 2
    # Format 3: two extra ID grids, then unknown tables, then screen metatile data.
    pos += w * h * 2 * 2
    pos += ua * 20 + ub * 8 + uc * 16
    return flags, w, h, count, ua, ub, uc, tsid, screen_ids, pos


def render_map(
    data: bytes, metatile: Image.Image, *, crop: bool
) -> Image.Image:
    flags, w, h, count, _ua, _ub, _uc, _tsid, screen_ids, pos = parse_lyr(data)
    mask = 0x03FF if flags in (0x0010, 0x0020) else (0x0FFF if flags == 0x0040 else 0x07FF)

    screens: list[Image.Image] = []
    blank: list[bool] = []
    for _ in range(count):
        screen = Image.new("RGBA", (256, 256), (0, 0, 0, 0))
        px = py = 0
        first_mid: int | None = None
        uniform = True
        for _t in range(256):
            mid = struct.unpack_from("<H", data, pos)[0] & mask
            pos += 2
            if first_mid is None:
                first_mid = mid
            elif mid != first_mid:
                uniform = False
            sx = (mid & 0x000F) * 16
            sy = mid & (0x03F0 if mask == 0x03FF else (0x0FF0 if mask == 0x0FFF else 0x07F0))
            tile = metatile.crop((sx, sy, sx + 16, sy + 16))
            screen.paste(tile, (px * 16, py * 16))
            px += 1
            if px == 16:
                px = 0
                py += 1
        # Screen 0 is reserved blank; any solid single-metatile screen is filler.
        blank.append(uniform)
        screens.append(screen)

    def is_content(sid: int) -> bool:
        return 0 <= sid < len(screens) and not blank[sid]

    full = Image.new("RGBA", (w * 256, h * 256), (0, 0, 0, 0))
    if crop:
        used = [(i, sid) for i, sid in enumerate(screen_ids) if is_content(sid)]
        if not used:
            return full
        cols = [i % w for i, _ in used]
        rows = [i // w for i, _ in used]
        x0, x1 = min(cols), max(cols)
        y0, y1 = min(rows), max(rows)
        cropped = Image.new(
            "RGBA", ((x1 - x0 + 1) * 256, (y1 - y0 + 1) * 256), (0, 0, 0, 0)
        )
        for i, sid in used:
            cx = (i % w) - x0
            cy = (i // w) - y0
            cropped.paste(screens[sid], (cx * 256, cy * 256))
        return cropped

    px = py = 0
    for sid in screen_ids:
        if is_content(sid):
            full.paste(screens[sid], (px * 256, py * 256))
        px += 1
        if px == w:
            px = 0
            py += 1
    return full


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--rpg-dir",
        type=Path,
        default=None,
        help="QuickBMS SigmaStarSaga_RPG dump directory",
    )
    parser.add_argument(
        "--out",
        type=Path,
        default=REPO / "graphics" / "overworld_maps",
        help="Output directory for map PNGs",
    )
    parser.add_argument(
        "--min-screens",
        type=int,
        default=MIN_SCREENS,
        help="Skip LYR files whose width*height is below this (default 4)",
    )
    parser.add_argument(
        "--no-crop",
        action="store_true",
        help="Keep full LYR dimensions including empty screen-0 padding",
    )
    parser.add_argument(
        "--save-metatiles",
        action="store_true",
        help="Also write tileset metatile sheets to <out>/_metatiles/",
    )
    args = parser.parse_args()

    rpg = find_rpg_dir(args.rpg_dir)
    args.out.mkdir(parents=True, exist_ok=True)
    for old in args.out.glob("*.png"):
        old.unlink()

    meta_cache: dict[int, Image.Image] = {}
    meta_dir = args.out / "_metatiles"
    if args.save_metatiles:
        meta_dir.mkdir(parents=True, exist_ok=True)

    lyrs = sorted(rpg.glob("*.lyr"), key=lambda p: int(p.stem))
    written = 0
    for lyr_path in lyrs:
        data = lyr_path.read_bytes()
        if len(data) < 20:
            continue
        flags, w, h, count, _ua, _ub, _uc, tsid, _ids, _off = parse_lyr(data)
        if w * h < args.min_screens:
            continue

        if tsid not in meta_cache:
            ts_path = rpg / f"{tsid}.ts4"
            if not ts_path.exists():
                ts_path = rpg / f"{tsid}.ts8"
            if not ts_path.exists():
                print(f"skip {lyr_path.name}: missing tileset {tsid}")
                continue
            scn_path = nearest_scn(rpg, tsid)
            if scn_path is None:
                print(f"skip {lyr_path.name}: no SCN for tileset {tsid}")
                continue
            palette = load_scn_bg_palette(scn_path)
            meta_cache[tsid] = build_metatile_sheet(ts_path, palette)
            if args.save_metatiles:
                meta_cache[tsid].save(meta_dir / f"{tsid}_metatile.png")

        image = render_map(data, meta_cache[tsid], crop=not args.no_crop)
        if image.getbbox() is None:
            print(f"skip {lyr_path.name}: empty after crop")
            continue

        name = f"{int(lyr_path.stem):03d}_{w}x{h}.png"
        image.save(args.out / name)
        print(
            f"{name} ({image.width}x{image.height}) "
            f"screens={count} tileset={tsid}"
        )
        written += 1

    print(f"Wrote {written} overworld maps to {args.out}")


if __name__ == "__main__":
    main()
