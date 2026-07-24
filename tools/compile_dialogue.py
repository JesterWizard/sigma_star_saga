#!/usr/bin/env python3
"""Compile src_custom/dialogue/*.c macros into APPEND_RODATA bank blobs.

Emits a C file with gDialogueBank0..6 (each ending in '#~\\0') for linking past
8MB. apply_lynjump.py redirects the vanilla bank pointer table when
runtime.custom_dialogue is TRUE.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]

CHAPTER_DIRS = [
    "chapter_00_system",
    "chapter_01",
    "chapter_02",
    "chapter_03",
    "chapter_04",
    "chapter_05",
    "chapter_06",
]

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

SPEAKER_BY_NAME = {
    "SPEAKER_" + n.upper(): i for i, n in enumerate(SPEAKER_NAMES)
}
SPEAKER_BY_NAME["SPEAKER_SILENCE"] = 0xFF

SIDE_BY_NAME = {"SIDE_LEFT": ord("<"), "SIDE_RIGHT": ord(">")}
EXPR_BY_NAME = {"EXPR_NEUTRAL": 0, "EXPR_ALT": 1}

SCRIPT_RE = re.compile(
    r"DIALOGUE_SCRIPT\s*\(\s*(0x[0-9A-Fa-f]+)\s*,\s*(\w+)\s*\)"
    r"(.*?)"
    r"END_DIALOGUE_SCRIPT\s*\(\s*\)",
    re.DOTALL,
)
# Macro call: NAME( ... ) with nested parens only in strings — keep simple.
CALL_RE = re.compile(
    r"\b(TALK|TEXT|CHAPTER_TITLE|EMPTY)\s*\(",
)


def decode_c_string(s: str) -> bytes:
    """Decode a C string literal body (no surrounding quotes) to bytes."""
    out = bytearray()
    i = 0
    while i < len(s):
        if s[i] != "\\":
            out.append(ord(s[i]))
            i += 1
            continue
        i += 1
        if i >= len(s):
            break
        ch = s[i]
        if ch == "n":
            out.append(0x0A)
        elif ch == "r":
            out.append(0x0D)
        elif ch == "t":
            out.append(0x09)
        elif ch == "0":
            out.append(0)
        elif ch == "x" and i + 2 < len(s):
            out.append(int(s[i + 1 : i + 3], 16))
            i += 2
        else:
            out.append(ord(ch))
        i += 1
    return bytes(out)


def split_c_args(argstr: str) -> list[str]:
    """Split a macro argument list on top-level commas."""
    args: list[str] = []
    cur: list[str] = []
    in_str = False
    escape = False
    depth = 0
    for ch in argstr:
        if in_str:
            cur.append(ch)
            if escape:
                escape = False
            elif ch == "\\":
                escape = True
            elif ch == '"':
                in_str = False
            continue
        if ch == '"':
            in_str = True
            cur.append(ch)
            continue
        if ch == "(":
            depth += 1
            cur.append(ch)
            continue
        if ch == ")":
            depth -= 1
            cur.append(ch)
            continue
        if ch == "," and depth == 0:
            args.append("".join(cur).strip())
            cur = []
            continue
        cur.append(ch)
    tail = "".join(cur).strip()
    if tail:
        args.append(tail)
    return args


def parse_string_arg(token: str) -> bytes:
    token = token.strip()
    if not (token.startswith('"') and token.endswith('"')):
        raise ValueError(f"expected string literal, got {token!r}")
    return decode_c_string(token[1:-1])


def resolve_speaker(token: str) -> int:
    token = token.strip()
    if token in SPEAKER_BY_NAME:
        return SPEAKER_BY_NAME[token]
    if token.startswith("SPEAKER_UNK_"):
        return int(token.split("_")[-1], 16)
    if token.startswith("0x") or token.isdigit():
        return int(token, 0)
    raise ValueError(f"unknown speaker {token}")


def resolve_side(token: str) -> int:
    token = token.strip()
    if token in SIDE_BY_NAME:
        return SIDE_BY_NAME[token]
    if len(token) == 3 and token[0] == "'" and token[2] == "'":
        return ord(token[1])
    raise ValueError(f"unknown side {token}")


def resolve_expr(token: str) -> int:
    token = token.strip()
    if token in EXPR_BY_NAME:
        return EXPR_BY_NAME[token]
    return int(token, 0)


def find_matching_paren(text: str, open_idx: int) -> int:
    """open_idx points at '('; return index of matching ')'."""
    depth = 0
    in_str = False
    escape = False
    for i in range(open_idx, len(text)):
        ch = text[i]
        if in_str:
            if escape:
                escape = False
            elif ch == "\\":
                escape = True
            elif ch == '"':
                in_str = False
            continue
        if ch == '"':
            in_str = True
            continue
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                return i
    raise ValueError("unbalanced parentheses in dialogue macro")


def encode_events(body: str) -> bytes:
    """Compile macro body (between DIALOGUE_SCRIPT and END) to entry bytes
    including the leading '#'."""
    # Strip C comments.
    body = re.sub(r"/\*.*?\*/", "", body, flags=re.DOTALL)
    body = re.sub(r"//.*?$", "", body, flags=re.MULTILINE)

    out = bytearray(b"#")
    pos = 0
    saw_content = False
    while True:
        m = CALL_RE.search(body, pos)
        if not m:
            break
        name = m.group(1)
        open_paren = m.end() - 1
        close = find_matching_paren(body, open_paren)
        argstr = body[open_paren + 1 : close]
        args = split_c_args(argstr) if argstr.strip() else []

        if name == "EMPTY":
            saw_content = True
            # bare '#' already written; nothing else
        elif name in ("TEXT", "CHAPTER_TITLE"):
            if len(args) < 1:
                raise ValueError(f"{name} needs at least one string")
            pages = [parse_string_arg(a) for a in args]
            for i, page in enumerate(pages):
                out.extend(page)
                out.append(0x0C)
            saw_content = True
        elif name == "TALK":
            if len(args) < 4:
                raise ValueError("TALK needs speaker, side, expr, and ≥1 page")
            speaker = resolve_speaker(args[0])
            side = resolve_side(args[1])
            expr = resolve_expr(args[2])
            pages = [parse_string_arg(a) for a in args[3:]]
            out.append(0x07)
            out.append(speaker & 0xFF)
            out.append(side & 0xFF)
            out.append(expr & 0xFF)
            out.append(0x07)
            for i, page in enumerate(pages):
                if i:
                    out.append(0x0C)
                out.extend(page)
            saw_content = True
        pos = close + 1

    if not saw_content:
        # Truly empty body → EMPTY stub
        pass
    return bytes(out)


def parse_scene_file(path: Path) -> tuple[int, bytes]:
    text = path.read_text(encoding="utf-8")
    m = SCRIPT_RE.search(text)
    if not m:
        raise ValueError(f"{path}: no DIALOGUE_SCRIPT … END_DIALOGUE_SCRIPT")
    addr = int(m.group(1), 16)
    body = m.group(3)
    return addr, encode_events(body)


def collect_banks(src_dir: Path) -> list[bytes]:
    banks: list[bytes] = []
    for dirname in CHAPTER_DIRS:
        chapter = src_dir / dirname
        if not chapter.is_dir():
            raise FileNotFoundError(f"missing chapter dir: {chapter}")
        scenes = sorted(chapter.glob("scene_*.c"))
        if not scenes:
            raise ValueError(f"no scenes in {chapter}")
        parts: list[tuple[int, bytes]] = []
        for path in scenes:
            addr, blob = parse_scene_file(path)
            parts.append((addr, blob))
        parts.sort(key=lambda t: t[0])
        bank = bytearray()
        for _, blob in parts:
            bank.extend(blob)
        bank.extend(b"#~\x00")
        banks.append(bytes(bank))
        print(f"  {dirname}: {len(parts)} scenes, {len(bank)} bytes")
    return banks


def c_byte_array(name: str, data: bytes) -> str:
    lines = [f"APPEND_RODATA const u8 {name}[] = {{"]
    for i in range(0, len(data), 16):
        chunk = data[i : i + 16]
        hexes = ", ".join(f"0x{b:02X}" for b in chunk)
        lines.append(f"    {hexes},")
    lines.append("};")
    lines.append(f"APPEND_RODATA const u32 {name}_size = sizeof({name});")
    return "\n".join(lines)


def emit_c(banks: list[bytes]) -> str:
    parts = [
        "/* Auto-generated by tools/compile_dialogue.py — do not edit. */",
        '#include "gba/types.h"',
        '#include "runtime.h"',
        "",
    ]
    for i, bank in enumerate(banks):
        parts.append(c_byte_array(f"gDialogueBank{i}", bank))
        parts.append("")
    parts.append("APPEND_RODATA const u8 * const gDialogueBanks[7] = {")
    for i in range(7):
        parts.append(f"    gDialogueBank{i},")
    parts.append("};")
    parts.append("")
    # Script-ID ranges for optional range-table patch (lo/hi inclusive).
    parts.append("APPEND_RODATA const u32 gDialogueIdRanges[14] = {")
    lo = 0
    for bank in banks:
        # Count '#' entries excluding terminator '#~'
        n = bank.count(ord("#")) - 1
        hi = lo + n - 1
        parts.append(f"    {lo}, {hi},")
        lo = hi + 1
    parts.append("};")
    parts.append("")
    return "\n".join(parts)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--src",
        type=Path,
        default=REPO / "src_custom" / "dialogue",
        help="Editable dialogue tree",
    )
    parser.add_argument(
        "--out",
        type=Path,
        required=True,
        help="Output dialogue_banks.c path",
    )
    args = parser.parse_args()

    if not args.src.is_dir():
        print(f"error: {args.src} not found", file=sys.stderr)
        return 1

    print(f"Compiling dialogue from {args.src}")
    banks = collect_banks(args.src)
    text = emit_c(banks)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(text, encoding="utf-8")
    total = sum(len(b) for b in banks)
    print(f"Wrote {args.out} ({total} bytes across 7 banks)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
