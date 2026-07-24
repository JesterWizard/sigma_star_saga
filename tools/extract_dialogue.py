#!/usr/bin/env python3
"""Dump Sigma Star Saga talk scripts from baserom.gba into data/dialogue/.

Each '#' entry (except bank terminator '#~') becomes one ygodm8-style .c macro
file, grouped by the 7 bank pointers at 0x24EA6C.

Requires: baserom.gba in the repo root.
"""

from __future__ import annotations

import argparse
import re
import shutil
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]

CHAPTER_PTR_TABLE = 0x24EA6C
CHAPTER_COUNT = 7
ID_RANGE_TABLE = 0x5BF3C

# Portrait ANM frame names; talk speaker id N ↔ index N.
SPEAKER_NAMES = [
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

CHAPTER_DIR_NAMES = [
    "chapter_00_system",
    "chapter_01",
    "chapter_02",
    "chapter_03",
    "chapter_04",
    "chapter_05",
    "chapter_06",
]

README = """# Dialogue dump (vanilla reference)

Vanilla talk scripts extracted from `baserom.gba` as ygodm8-style C macros.

**Editable copy for in-game patches:** [`src_custom/dialogue/`](../../src_custom/dialogue/).
Full write-up: [`documentation/dialogue/`](../../documentation/dialogue/).

## Regenerate (overwrites this tree only)

```bash
python3 tools/extract_dialogue.py
```

To refresh the editable copy from vanilla (destroys local edits):

```bash
rm -rf src_custom/dialogue
cp -a data/dialogue src_custom/dialogue
```

## Layout

Seven banks from the pointer table at `0x24EA6C` (ids 0–464). Each
`scene_XXXXXX.c` is one `#` entry; `XXXXXX` is the file offset of that `#`.
Empty stubs are kept as `EMPTY()` so script IDs stay aligned.
"""


def speaker_macro(speaker_id: int) -> str:
    if speaker_id == 0xFF:
        return "SPEAKER_SILENCE"
    if 0 <= speaker_id < len(SPEAKER_NAMES):
        return "SPEAKER_" + SPEAKER_NAMES[speaker_id].upper()
    return f"SPEAKER_UNK_{speaker_id:02X}"


def expr_macro(expr: int) -> str:
    if expr == 0:
        return "EXPR_NEUTRAL"
    if expr == 1:
        return "EXPR_ALT"
    return str(expr)


def side_macro(side: int) -> str:
    if side == ord("<"):
        return "SIDE_LEFT"
    if side == ord(">"):
        return "SIDE_RIGHT"
    return repr(chr(side) if 32 <= side < 127 else side)


def c_string(text: str) -> str:
    escaped = (
        text.replace("\\", "\\\\")
        .replace('"', '\\"')
        .replace("\n", "\\n")
        .replace("\r", "\\r")
        .replace("\t", "\\t")
    )
    return f'"{escaped}"'


def decode_text_bytes(data: bytes) -> str:
    out: list[str] = []
    for b in data:
        if b == 0x0A:
            out.append("\n")
        elif 32 <= b < 127:
            out.append(chr(b))
        elif b == 0:
            continue
        else:
            out.append(f"\\x{b:02x}")
    return "".join(out)


def is_talk_header(data: bytes, i: int) -> bool:
    return (
        i + 4 < len(data)
        and data[i] == 0x07
        and data[i + 2] in (ord("<"), ord(">"))
        and data[i + 4] == 0x07
    )


def split_pages(raw: bytes) -> list[str]:
    parts = raw.split(b"\x0c")
    pages: list[str] = []
    for part in parts:
        text = decode_text_bytes(part).strip("\0")
        if text == "":
            continue
        pages.append(text)
    return pages


def looks_like_chapter_title(text: str) -> bool:
    t = text.strip()
    return bool(re.match(r"(?i)^chapter\s+\d+", t))


def parse_entry(blob: bytes) -> list[tuple]:
    """Parse one '#' entry (blob includes leading '#')."""
    assert blob[:1] == b"#"
    body = blob[1:]
    # Empty / whitespace-only stub (still occupies a script ID).
    if body.strip(b" \t\0") == b"":
        return [("empty",)]

    events: list[tuple] = []
    i = 0

    while i < len(body):
        if is_talk_header(body, i):
            speaker = body[i + 1]
            side = body[i + 2]
            expr = body[i + 3]
            i += 5
            start = i
            while i < len(body) and not is_talk_header(body, i):
                i += 1
            pages = split_pages(body[start:i])
            if not pages:
                pages = [""]
            events.append(("talk", speaker, side, expr, pages))
        else:
            start = i
            while i < len(body) and not is_talk_header(body, i):
                i += 1
            chunk = body[start:i]
            pages = split_pages(chunk)
            if not pages:
                text = decode_text_bytes(chunk).strip()
                if text in ("", "?"):
                    continue
                pages = [text]
            for page in pages:
                page = page.strip()
                if not page or page == "?":
                    continue
                if looks_like_chapter_title(page):
                    events.append(("chapter_title", page))
                else:
                    events.append(("text", page))
    return events if events else [("empty",)]


def format_call(kind: str, pages: list[str], indent: str = "  ") -> str:
    if len(pages) == 1:
        return f"{indent}{kind}({c_string(pages[0])})"
    lines = [f"{indent}{kind}("]
    for idx, page in enumerate(pages):
        comma = "," if idx < len(pages) - 1 else ""
        lines.append(f"{indent}    {c_string(page)}{comma}")
    lines.append(f"{indent})")
    return "\n".join(lines)


def emit_c(rom_addr: int, file_off: int, events: list[tuple]) -> str:
    name = f"scene_{file_off:06X}"
    lines = [
        '#include "dialogue_macros.h"',
        "",
        f"/* ROM 0x{rom_addr:08X} */",
        f"DIALOGUE_SCRIPT(0x{rom_addr:08X}, {name})",
    ]
    for ev in events:
        if ev[0] == "empty":
            lines.append("  EMPTY()")
        elif ev[0] == "chapter_title":
            lines.append(format_call("CHAPTER_TITLE", [ev[1]]))
        elif ev[0] == "text":
            lines.append(format_call("TEXT", [ev[1]]))
        elif ev[0] == "talk":
            _, speaker, side, expr, pages = ev
            sp = speaker_macro(speaker)
            sd = side_macro(side)
            ex = expr_macro(expr)
            if len(pages) == 1:
                lines.append(
                    f"  TALK({sp}, {sd}, {ex},\n"
                    f"      {c_string(pages[0])})"
                )
            else:
                lines.append(f"  TALK({sp}, {sd}, {ex},")
                for idx, page in enumerate(pages):
                    comma = "," if idx < len(pages) - 1 else ""
                    lines.append(f"      {c_string(page)}{comma}")
                lines.append("  )")
    lines.append("END_DIALOGUE_SCRIPT()")
    lines.append("")
    return "\n".join(lines)


def chapter_bounds(rom: bytes) -> list[tuple[str, int, int]]:
    ptrs = [
        struct.unpack_from("<I", rom, CHAPTER_PTR_TABLE + i * 4)[0] - 0x08000000
        for i in range(CHAPTER_COUNT)
    ]
    bounds: list[tuple[str, int, int]] = []
    for i in range(CHAPTER_COUNT):
        start = ptrs[i]
        if i + 1 < CHAPTER_COUNT:
            end = ptrs[i + 1]
        else:
            # End of bank 6: just past '#~\x00'
            tilde = rom.find(b"#~", start)
            end = tilde + 3 if tilde >= 0 else start
        bounds.append((CHAPTER_DIR_NAMES[i], start, end))
    return bounds


def iter_hash_entries(rom: bytes, start: int, end: int):
    """Yield (#offset, blob) for each entry; stop at bank terminator '#~'."""
    i = start
    while i < end:
        if rom[i] == ord("#"):
            if i + 1 < end and rom[i + 1] == ord("~"):
                return
            j = i + 1
            while j < end and rom[j] != ord("#"):
                j += 1
            yield i, rom[i:j]
            i = j
        else:
            i += 1


def dump_dialogue(rom: bytes, out_dir: Path) -> int:
    if out_dir.exists():
        for child in out_dir.iterdir():
            if child.is_dir() and child.name.startswith("chapter_"):
                shutil.rmtree(child)
            elif child.name == "README.md":
                child.unlink()

    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / "README.md").write_text(README, encoding="utf-8")

    total = 0
    for dirname, start, end in chapter_bounds(rom):
        chapter_dir = out_dir / dirname
        chapter_dir.mkdir(parents=True, exist_ok=True)
        count = 0
        for file_off, blob in iter_hash_entries(rom, start, end):
            events = parse_entry(blob)
            rom_addr = 0x08000000 + file_off
            text = emit_c(rom_addr, file_off, events)
            path = chapter_dir / f"scene_{file_off:06X}.c"
            path.write_text(text, encoding="utf-8")
            count += 1
            total += 1
        print(f"  {dirname}: {count} scenes (0x{start:06X}–0x{end:06X})")
    return total


def main() -> int:
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
        default=REPO / "data" / "dialogue",
        help="Output directory (default: data/dialogue)",
    )
    args = parser.parse_args()

    if not args.rom.is_file():
        print(f"error: ROM not found: {args.rom}", file=sys.stderr)
        return 1

    rom = args.rom.read_bytes()
    print(f"Dumping dialogue from {args.rom} → {args.out}")
    total = dump_dialogue(rom, args.out)
    print(f"Wrote {total} scene files")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
