#!/usr/bin/env python3
"""Compile src_custom/dialogue/*.c macros into APPEND_RODATA bank blobs.

Emits a C file with gDialogueBank0..6 (each ending in '#~\\0') for linking past
8MB. apply_lynjump.py redirects the vanilla bank pointer table when
runtime.custom_dialogue is TRUE.

Optional TALK voice cues (VOICE(id) / VOICE_STOP) emit gTalkVoiceCues[] for
TalkAdvance hooks when .custom_gax_audio is on.
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
    r"(?:DIALOGUE_SCRIPT|EVENT_SCRIPT_REPLACEMENT)\s*\(\s*(0x[0-9A-Fa-f]+)\s*,\s*(\w+)"
    r"[^)]*\)"
    r"(.*?)"
    r"(?:END_DIALOGUE_SCRIPT|END_EVENT_SCRIPT)\s*\(\s*\)",
    re.DOTALL,
)
# Event choreography macros are ignored; only talk-bank ops emit bytes.
CALL_RE = re.compile(
    r"\b(TALK|TEXT|CHAPTER_TITLE|CHOICE|EMPTY)\s*\(",
)
VOICE_CALL_RE = re.compile(r"^VOICE\s*\(\s*(.+)\s*\)$")
VOICE_STOP_ID = 0xFFFF


def load_voice_name_map() -> dict[str, int]:
    """GAX_VOICE_* from include/gax_catalog.h (may be empty before first catalog build)."""
    path = REPO / "include" / "gax_catalog.h"
    names: dict[str, int] = {"VOICE_STOP": VOICE_STOP_ID}
    if not path.is_file():
        return names
    for m in re.finditer(r"#define\s+(GAX_VOICE_\w+)\s+(\d+)", path.read_text()):
        names[m.group(1)] = int(m.group(2))
    return names


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


def is_string_arg(token: str) -> bool:
    t = token.strip()
    return len(t) >= 2 and t[0] == '"' and t[-1] == '"'


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


def resolve_voice_arg(token: str, voice_names: dict[str, int]) -> int:
    token = token.strip()
    if token == "VOICE_STOP":
        return VOICE_STOP_ID
    m = VOICE_CALL_RE.match(token)
    if not m:
        raise ValueError(f"expected VOICE(id) or VOICE_STOP, got {token!r}")
    inner = m.group(1).strip()
    if inner in voice_names:
        return voice_names[inner]
    if inner.startswith("0x") or inner.isdigit():
        return int(inner, 0) & 0xFFFF
    raise ValueError(f"unknown voice id {inner!r}")


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


def encode_events(
    body: str, voice_names: dict[str, int]
) -> tuple[bytes, list[tuple[int, int]]]:
    """Compile macro body to entry bytes (leading '#') + voice cues.

    Each cue is (offset_within_entry, voice_id) pointing at the TALK header 0x07.
    """
    body = re.sub(r"/\*.*?\*/", "", body, flags=re.DOTALL)
    body = re.sub(r"//.*?$", "", body, flags=re.MULTILINE)

    out = bytearray(b"#")
    cues: list[tuple[int, int]] = []
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
        elif name == "CHOICE":
            if not out.endswith(b"\x0c"):
                out.append(0x0C)
            out.append(ord("?"))
            saw_content = True
        elif name in ("TEXT", "CHAPTER_TITLE"):
            if len(args) < 1:
                raise ValueError(f"{name} needs at least one string")
            pages = [parse_string_arg(a) for a in args]
            for page in pages:
                out.extend(page)
                out.append(0x0C)
            saw_content = True
        elif name == "TALK":
            if len(args) < 4:
                raise ValueError(
                    "TALK needs speaker, side, expr, optional VOICE(...), and ≥1 page"
                )
            speaker = resolve_speaker(args[0])
            side = resolve_side(args[1])
            expr = resolve_expr(args[2])
            i = 3
            voice: int | None = None
            while i < len(args) and not is_string_arg(args[i]):
                voice = resolve_voice_arg(args[i], voice_names)
                i += 1
            if i >= len(args):
                raise ValueError("TALK needs ≥1 page string")
            pages = [parse_string_arg(a) for a in args[i:]]
            header_off = len(out)
            out.append(0x07)
            out.append(speaker & 0xFF)
            out.append(side & 0xFF)
            out.append(expr & 0xFF)
            out.append(0x07)
            for pi, page in enumerate(pages):
                if pi:
                    out.append(0x0C)
                out.extend(page)
            if voice is not None:
                cues.append((header_off, voice))
            saw_content = True
        pos = close + 1

    del saw_content  # kept for symmetry with prior EMPTY handling
    return bytes(out), cues


def parse_scene_file(
    path: Path, voice_names: dict[str, int]
) -> tuple[int, bytes, list[tuple[int, int]]]:
    text = path.read_text(encoding="utf-8")
    m = SCRIPT_RE.search(text)
    if not m:
        raise ValueError(
            f"{path}: no DIALOGUE_SCRIPT/EVENT_SCRIPT_REPLACEMENT … END_* block"
        )
    addr = int(m.group(1), 16)
    blob, cues = encode_events(m.group(3), voice_names)
    return addr, blob, cues


def collect_banks(
    src_dir: Path, voice_names: dict[str, int]
) -> tuple[list[bytes], list[tuple[int, int, int]]]:
    """Returns banks and cues as (bank_index, byte_offset, voice_id)."""
    banks: list[bytes] = []
    all_cues: list[tuple[int, int, int]] = []
    for bank_i, dirname in enumerate(CHAPTER_DIRS):
        chapter = src_dir / dirname
        if not chapter.is_dir():
            raise FileNotFoundError(f"missing chapter dir: {chapter}")
        scenes = sorted(chapter.glob("scene_*.c"))
        if not scenes:
            raise ValueError(f"no scenes in {chapter}")
        parts: list[tuple[int, bytes, list[tuple[int, int]]]] = []
        for path in scenes:
            addr, blob, cues = parse_scene_file(path, voice_names)
            parts.append((addr, blob, cues))
        parts.sort(key=lambda t: t[0])
        bank = bytearray()
        for _, blob, cues in parts:
            base = len(bank)
            bank.extend(blob)
            for off, vid in cues:
                all_cues.append((bank_i, base + off, vid))
        bank.extend(b"#~\x00")
        banks.append(bytes(bank))
        print(f"  {dirname}: {len(parts)} scenes, {len(bank)} bytes")
    return banks, all_cues


def c_byte_array(name: str, data: bytes) -> str:
    lines = [f"APPEND_RODATA const u8 {name}[] = {{"]
    for i in range(0, len(data), 16):
        chunk = data[i : i + 16]
        hexes = ", ".join(f"0x{b:02X}" for b in chunk)
        lines.append(f"    {hexes},")
    lines.append("};")
    lines.append(f"APPEND_RODATA const u32 {name}_size = sizeof({name});")
    return "\n".join(lines)


def emit_c(banks: list[bytes], cues: list[tuple[int, int, int]]) -> str:
    parts = [
        "/* Auto-generated by tools/compile_dialogue.py — do not edit. */",
        '#include "gba/types.h"',
        '#include "runtime.h"',
        "",
        "/* Mirrors TalkVoiceCue in include/dialogue.h (avoid gDialogueIdRanges macro). */",
        "typedef struct {",
        "    const u8 *header;",
        "    u16 voiceId;",
        "} TalkVoiceCue;",
        "",
        "extern const TalkVoiceCue gTalkVoiceCues[];",
        "extern const u16 gTalkVoiceCueCount;",
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
    parts.append("APPEND_RODATA const u32 gDialogueIdRanges[14] = {")
    lo = 0
    for bank in banks:
        n = bank.count(ord("#")) - 1
        hi = lo + n - 1
        parts.append(f"    {lo}, {hi},")
        lo = hi + 1
    parts.append("};")
    parts.append("")

    parts.append(f"APPEND_RODATA const u16 gTalkVoiceCueCount = {len(cues)};")
    parts.append("APPEND_RODATA const TalkVoiceCue gTalkVoiceCues[] = {")
    if not cues:
        parts.append("    {0, 0},")
    else:
        for bank_i, off, vid in cues:
            parts.append(
                f"    {{ &gDialogueBank{bank_i}[{off}], 0x{vid:04X} }},"
            )
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

    voice_names = load_voice_name_map()
    print(f"Compiling dialogue from {args.src}")
    banks, cues = collect_banks(args.src, voice_names)
    text = emit_c(banks, cues)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(text, encoding="utf-8")
    total = sum(len(b) for b in banks)
    print(f"Wrote {args.out} ({total} bytes across 7 banks, {len(cues)} voice cues)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
