#!/usr/bin/env python3
"""Build append-ROM GAX music/voice catalogs from sound/.

Reads:
  sound/music/*.json
  sound/voice/*.json   (each points at a .bin from pack_gax_speech)

Writes:
  src_custom/generated/gax_catalog.c
  src_custom/generated/gax_catalog.h
"""

from __future__ import annotations

import argparse
import json
import re
import struct
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
SOUND = REPO / "sound"
OUT_DIR = REPO / "src_custom" / "generated"
OUT_H_DIR = REPO / "include"
PACK_SPEECH = REPO / "tools" / "pack_gax_speech.py"
PACK_SONG = REPO / "tools" / "pack_gax_song.py"
PACK_VOICE_FX = REPO / "tools" / "pack_gax_voice_fx.py"
BASEROM = REPO / "baserom.gba"

# Vanilla GAX FX module layout (see documentation/gax-audio.md). gax_fx(id)
# sequences package+0x10 list[id] on an FX channel; instruments read samples
# from the package+0x14 wave set. Voice rides appended entries past these.
GAX_PACKAGE_ADDR = 0x0824BE44
GAX_PACKAGE_BYTES = 0x20
GAX_FX_LIST_ADDR = 0x0814F4E0
GAX_FX_LIST_COUNT = 124
GAX_FX_WAVESET_ADDR = 0x0824B8DC
GAX_FX_WAVESET_COUNT = 175
GAX_FX_ENTRY_BYTES = 0x44
GAX_FX_DATA_BYTES = 0x14  # volenv(12) + perf row(8) block at entry-0x14
GAX_FX_NULL_ENTRY_ADDR = 0x0814D800  # shared vanilla is_null entry (id 0)
# Fixed perf note for voice FX. 0x3D ≈ 1:1 at 15769 Hz (15789). Tuned three
# semitones lower (0x3A, ~16% slower/deeper) so in-game pitch matches source.
VOICE_FX_NOTE = 0x3A

# Vanilla FX entry template (0x44 body) + data block (0x14), recovered from
# baserom @ 0x0814EB0C..0x0814EB64 (entry 1). All vanilla FX entries share it;
# only waveSlots[0] (+0x01) and the perf note differ per sound.
GAX_FX_VOLENV = bytes([0x01, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0, 0, 0])
GAX_FX_PERF_ROW = bytes([0x37, 0x01, 0x01, 0, 0, 0, 0, 0])


def extract_vanilla_fx() -> tuple[bytes, list[int], list[tuple[int, int]]]:
    rom = BASEROM.read_bytes()

    def at(addr: int, n: int) -> bytes:
        return rom[addr - 0x08000000 : addr - 0x08000000 + n]

    pkg = at(GAX_PACKAGE_ADDR, GAX_PACKAGE_BYTES)
    lst = list(
        struct.unpack(f"<{GAX_FX_LIST_COUNT}I", at(GAX_FX_LIST_ADDR, GAX_FX_LIST_COUNT * 4))
    )
    waves = [
        struct.unpack("<II", at(GAX_FX_WAVESET_ADDR + i * 8, 8))
        for i in range(GAX_FX_WAVESET_COUNT)
    ]
    return pkg, lst, waves


def parse_int(s) -> int:
    if isinstance(s, int):
        return s
    return int(str(s), 0)


def c_ident(name: str) -> str:
    s = re.sub(r"[^0-9A-Za-z_]", "_", name)
    if s and s[0].isdigit():
        s = "_" + s
    return s


_VOICE_FILE_INDEX_RE = re.compile(r"^(\d+)[_-]")


def voice_constant_name(meta: dict, path: Path) -> str:
    """C macro name for this clip — must match VOICE(GAX_VOICE_…) in dialogue.

    JSON `id` is the constant (with or without the GAX_VOICE_ prefix). Legacy
    numeric `id` + `name` still works.
    """
    raw = meta.get("id")
    if isinstance(raw, str) and not _is_numeric_token(raw):
        token = raw.strip()
        if token.startswith("GAX_VOICE_"):
            token = token[len("GAX_VOICE_") :]
        return "GAX_VOICE_" + c_ident(token).upper()
    name = meta.get("name")
    if name:
        return "GAX_VOICE_" + c_ident(str(name)).upper()
    raise ValueError(
        f"{path}: id must be a GAX_VOICE_* constant name "
        f'(e.g. "GAX_VOICE_TIERNEY_BRIEFING"), or provide name with numeric id'
    )


def _is_numeric_token(s: str) -> bool:
    t = s.strip().lower()
    if t.startswith("0x"):
        return all(c in "0123456789abcdef" for c in t[2:]) and len(t) > 2
    return t.isdigit()


def voice_numeric_id(meta: dict, path: Path) -> int:
    """Numeric catalog slot: explicit `index`, else NN_ from filename, else legacy numeric id."""
    if "index" in meta:
        return parse_int(meta["index"])
    m = _VOICE_FILE_INDEX_RE.match(path.stem)
    if m:
        return int(m.group(1), 10)
    raw = meta.get("id")
    if isinstance(raw, int) or (isinstance(raw, str) and _is_numeric_token(raw)):
        return parse_int(raw)
    raise ValueError(
        f"{path}: need numeric slot — use filename prefix NN_name.json "
        f'or set "index": N'
    )


def collect_music() -> list[dict]:
    items = []
    music_dir = SOUND / "music"
    if not music_dir.is_dir():
        return items
    for path in sorted(music_dir.glob("*.json")):
        meta = json.loads(path.read_text())
        meta["_path"] = path
        items.append(meta)
    items.sort(key=lambda m: parse_int(m.get("id", 0)))
    return items


def voice_audio_path(meta: dict, json_path: Path) -> Path:
    """Resolve source audio: prefer `src`, then `mp3`, then `wav`."""
    for key in ("src", "mp3", "wav"):
        if key in meta and meta[key]:
            return json_path.parent / meta[key]
    raise KeyError(f"{json_path}: need src, mp3, or wav")


def collect_voice() -> list[dict]:
    items = []
    voice_dir = SOUND / "voice"
    if not voice_dir.is_dir():
        return items
    seen_const: dict[str, Path] = {}
    seen_index: dict[int, Path] = {}
    for path in sorted(voice_dir.glob("*.json")):
        # Skip inventory / non-manifest JSON (e.g. a leftover VOICES.json).
        if path.name.upper() in {"VOICES.JSON"}:
            continue
        meta = json.loads(path.read_text())
        if not isinstance(meta, dict) or (
            "src" not in meta and "mp3" not in meta and "wav" not in meta
        ):
            continue
        meta["_path"] = path
        meta["_constant"] = voice_constant_name(meta, path)
        meta["_id"] = voice_numeric_id(meta, path)
        if meta["_constant"] in seen_const:
            raise ValueError(
                f"{path}: duplicate constant {meta['_constant']} "
                f"(also {seen_const[meta['_constant']]})"
            )
        if meta["_id"] in seen_index:
            raise ValueError(
                f"{path}: duplicate index {meta['_id']} "
                f"(also {seen_index[meta['_id']]})"
            )
        seen_const[meta["_constant"]] = path
        seen_index[meta["_id"]] = path
        audio = voice_audio_path(meta, path)
        if not audio.is_file():
            raise FileNotFoundError(f"{path}: missing audio {audio}")
        stem = path.parent / "built" / path.stem
        stem.parent.mkdir(parents=True, exist_ok=True)
        subprocess.check_call(
            [sys.executable, str(PACK_SPEECH), str(audio), "-o", str(stem)]
        )
        entry = json.loads(stem.with_suffix(".entry.json").read_text())
        bin_path = stem.with_suffix(".bin")
        meta["_bin"] = bin_path
        meta["_entry"] = entry
        fx_pcm = stem.with_suffix(".fx.s8")
        pitch = meta.get("pitch_semitones", None)
        fx_cmd = [sys.executable, str(PACK_VOICE_FX), str(audio), "-o", str(fx_pcm)]
        if pitch is not None:
            fx_cmd += ["--pitch-semitones", str(pitch)]
        subprocess.check_call(fx_cmd)
        meta["_fx_pcm"] = fx_pcm
        items.append(meta)
    items.sort(key=lambda m: m["_id"])
    return items


def _fmt_bytes(n: int) -> str:
    return f"{n:,} B ({n / 1024:.1f} KB)"


def _change_pct(in_rom: int, source: int) -> float | None:
    if source <= 0:
        return None
    return round(100.0 * (in_rom - source) / source, 1)


def _change_html(pct: float | None) -> str:
    if pct is None:
        return "n/a"
    color = "#3fb950" if pct < 0 else "#f85149"
    return f'<span style="color: {color}">{pct:+.1f}%</span>'


# Per-clip FX instrument overhead embedded beside the PCM wave payload.
FX_ENTRY_OVERHEAD = GAX_FX_DATA_BYTES + GAX_FX_ENTRY_BYTES  # 0x14 + 0x44
FX_LIST_PTR_BYTES = 4
SPEECH_TABLE_ENTRY_BYTES = 8


def write_voice_inventory(voice: list[dict]) -> Path:
    """Write sound/voice/VOICES.md — source vs in-ROM sizes (ygodm8-style)."""
    voice_dir = SOUND / "voice"
    rows = []
    sum_src = 0
    sum_rom = 0
    registered_audio: set[Path] = set()

    for v in voice:
        audio = voice_audio_path(v, v["_path"])
        registered_audio.add(audio.resolve())
        src_n = audio.stat().st_size
        fx_n = Path(v["_fx_pcm"]).stat().st_size
        speech_n = Path(v["_bin"]).stat().st_size
        overhead = FX_ENTRY_OVERHEAD + FX_LIST_PTR_BYTES + SPEECH_TABLE_ENTRY_BYTES
        rom_n = fx_n + speech_n + overhead
        pct = _change_pct(rom_n, src_n)
        sum_src += src_n
        sum_rom += rom_n
        title = v.get("_constant", v.get("name", audio.stem))
        rows.append(
            (
                title,
                audio.name,
                v["_path"].name,
                _fmt_bytes(src_n),
                _fmt_bytes(rom_n),
                _change_html(pct),
            )
        )

    unregistered = []
    for path in sorted(voice_dir.iterdir()):
        if not path.is_file():
            continue
        if path.suffix.lower() not in {".wav", ".mp3", ".ogg", ".flac", ".aiff"}:
            continue
        if path.resolve() in registered_audio:
            continue
        unregistered.append((path.name, _fmt_bytes(path.stat().st_size)))

    total_pct = _change_pct(sum_rom, sum_src)
    lines = [
        "# Custom Voice Asset Inventory",
        "",
        "Auto-generated by [`tools/build_gax_catalog.py`](../../tools/build_gax_catalog.py) when you rebuild the GAX catalog.",
        "",
        "Sizes:",
        "",
        "- **Source** — on-disk source clip (`.mp3` / `.wav` / …), including container headers.",
        "- **In-ROM** — FX unsigned-8 PCM payload + FX entry (`0x58`) + FX list pointer (`4`) "
        "+ speech bitstream blob + speech table entry (`8`).",
        "- **Overall Change** — `(in-ROM − source) / source`; negative means the ROM blob is smaller.",
        "- **Total** row — sum of all registered clips (source vs in-ROM).",
        "- Playback uses raw **u8 PCM @ 15769 Hz** via the GAX FX path (speech vocoder blob is also linked).",
        "- JSON `id` is the C constant (`GAX_VOICE_…`); numeric slot comes from the `NN_` filename prefix or `index`.",
        "",
        "## Registered clips",
        "",
        "<table>",
        "  <thead>",
        "    <tr>",
        "      <th>Constant</th>",
        "      <th>Source</th>",
        "      <th>Manifest</th>",
        "      <th>Source Size</th>",
        "      <th>In-ROM</th>",
        "      <th>Overall Change</th>",
        "    </tr>",
        "  </thead>",
        "  <tbody>",
    ]
    for title, src, manifest, src_d, rom_d, chg in rows:
        lines.append(
            f"    <tr><td><code>{title}</code></td><td><code>{src}</code></td>"
            f"<td><code>{manifest}</code></td><td>{src_d}</td>"
            f"<td>{rom_d}</td><td>{chg}</td></tr>"
        )
    lines.append(
        f'    <tr><td colspan="3"><strong>Total</strong></td>'
        f"<td><strong>{_fmt_bytes(sum_src)}</strong></td>"
        f"<td><strong>{_fmt_bytes(sum_rom)}</strong></td>"
        f"<td><strong>{_change_html(total_pct)}</strong></td></tr>"
    )
    lines += [
        "  </tbody>",
        "</table>",
        "",
        f"_Sample rate: 15769 Hz · FX note `0x{VOICE_FX_NOTE:02X}` · "
        f"{len(rows)} clip(s) from `sound/voice/*.json`._",
        "",
    ]
    if unregistered:
        lines += [
            "## Unregistered audio",
            "",
            "These files exist under `sound/voice/` but are not listed in a manifest:",
            "",
        ]
        for name, size in unregistered:
            lines.append(f"- `{name}` ({size})")
        lines.append("")

    out = voice_dir / "VOICES.md"
    out.write_text("\n".join(lines))
    json_legacy = voice_dir / "VOICES.json"
    if json_legacy.exists():
        json_legacy.unlink()
    print(f"wrote {out} ({len(rows)} clip(s))")
    return out


def emit_voice_fx(lines: list[str], voice_slots: list[dict | None]) -> None:
    """Append FX-path voice playback structures.

    gax_fx(id) sequences package+0x10 list[id] on an FX channel. Each voice
    clip gets a 0x44-byte FX entry (instrument + embedded volenv/perf/wave
    params, layout recovered from vanilla @ 0x0814D800+i*0x44) plus a wave
    set slot holding its unsigned 8-bit PCM (0x80=silence) at the FX mix rate.
    """
    pkg, vanilla_list, vanilla_waves = extract_vanilla_fx()
    n_slots = len(voice_slots)

    lines.append("/* FX voice path: extended package + module list + wave set.")
    lines.append(" * Vanilla FX module layout (baserom @ 0x0814D7EC..): a 0x14-byte data")
    lines.append(" * block (12B volenv + 8B perf row) immediately precedes each 0x44-byte")
    lines.append(" * entry; the entry's +0x0C/+0x14 pointers reference that block. Voice")
    lines.append(" * entries clone the vanilla template with waveSlots[0] -> voice PCM. */")
    lines.append("typedef struct { const u8 *addr; u32 size; } GaxFxWaveEntry;")
    lines.append(
        "typedef struct {\n"
        "    u8 volenv[12];\n"
        "    u8 perfRow[8];\n"
        "} GaxFxVoiceData; /* 0x14 bytes, sits at entry-0x14 in vanilla packing */"
    )
    lines.append(
        "typedef struct {\n"
        "    u8 isNull;\n"
        "    u8 waveSlots[4];\n"
        "    u8 pad5[7];\n"
        "    const u8 *volenvPtr;\n"
        "    u8 perfRowSpeed;\n"
        "    u8 perfListLen;\n"
        "    u16 unk12;\n"
        "    const u8 *perfListPtr;\n"
        "    u8 waveParams[24];\n"
        "    u8 tail[20]; /* vanilla: next module's data block; unused for voice */"
        "\n} GaxFxVoiceEntry; /* 0x44 bytes, must stay 4-aligned */"
    )
    lines.append("")

    for i, v in enumerate(voice_slots):
        if v is None:
            continue
        vid = v["_id"]
        pcm = Path(v["_fx_pcm"]).read_bytes()
        lines.append(f"APPEND_RODATA static const u8 sVoiceFxPcm_{vid}[] = {{")
        for j in range(0, len(pcm), 16):
            chunk = ", ".join(str(b) for b in pcm[j : j + 16])
            lines.append(f"    {chunk},")
        lines.append("};")

    for i, v in enumerate(voice_slots):
        if v is None:
            continue
        volenv = ", ".join(str(b) for b in GAX_FX_VOLENV)
        perf = [VOICE_FX_NOTE] + list(GAX_FX_PERF_ROW[1:])
        perf_s = ", ".join(str(b) for b in perf)
        lines.append(f"APPEND_RODATA static const GaxFxVoiceData sVoiceFxData_{i} = {{")
        lines.append(f"    .volenv = {{ {volenv} }},")
        lines.append(f"    .perfRow = {{ {perf_s} }},")
        lines.append("};")
    lines.append("")

    # The FX engine rejects wave set indices beyond the vanilla table, so
    # voice PCM occupies unused {0,0} holes in the copied vanilla wave set.
    hole_slots = [
        i for i, (addr, size) in enumerate(vanilla_waves) if addr == 0 and size == 0
    ]

    n_entries = max(n_slots, 1)
    lines.append(
        f"APPEND_RODATA static const GaxFxVoiceEntry sVoiceFxEntries[{n_entries}] = {{"
    )
    for i, v in enumerate(voice_slots):
        if v is None:
            lines.append("    { .isNull = 1 },")
            continue
        wave_idx = hole_slots[i]
        lines.append("    {")
        lines.append(f"        .waveSlots = {{ {wave_idx}, 0, 0, 0 }},")
        lines.append(f"        .volenvPtr = sVoiceFxData_{i}.volenv,")
        lines.append("        .perfRowSpeed = 1,")
        lines.append("        .perfListLen = 1,")
        lines.append(f"        .perfListPtr = sVoiceFxData_{i}.perfRow,")
        lines.append("    },")
    lines.append("};")
    lines.append("")

    voice_wave = {}
    for i, v in enumerate(voice_slots):
        if v is not None:
            vid = v["_id"]
            voice_wave[hole_slots[i]] = vid

    n_waves = GAX_FX_WAVESET_COUNT
    lines.append(f"APPEND_RODATA static const GaxFxWaveEntry gGaxFxWaveSetEx[{n_waves}] = {{")
    for idx, (addr, size) in enumerate(vanilla_waves):
        if idx in voice_wave:
            vid = voice_wave[idx]
            lines.append(f"    {{ (const u8 *)sVoiceFxPcm_{vid}, sizeof(sVoiceFxPcm_{vid}) }},")
        else:
            a = f"(const u8 *)0x{addr:08X}" if addr else "0"
            lines.append(f"    {{ {a}, 0x{size:08X} }},")
    lines.append("};")
    lines.append("")

    n_list = GAX_FX_LIST_COUNT + n_entries
    lines.append(f"APPEND_RODATA static const GaxFxVoiceEntry *const gGaxFxListEx[{n_list}] = {{")
    for addr in vanilla_list:
        lines.append(f"    (const GaxFxVoiceEntry *)0x{addr:08X},")
    for i, v in enumerate(voice_slots):
        if v is None:
            lines.append(f"    (const GaxFxVoiceEntry *)0x{GAX_FX_NULL_ENTRY_ADDR:08X},")
        else:
            lines.append(f"    &sVoiceFxEntries[{i}],")
    lines.append("};")
    lines.append("")

    pkg_words = struct.unpack(f"<{GAX_PACKAGE_BYTES // 4}I", pkg)
    lines.append(f"APPEND_RODATA const u32 gGaxPackageEx[{len(pkg_words)}] = {{")
    for w in pkg_words[:4]:
        lines.append(f"    0x{w:08X},")
    lines.append("    (u32)gGaxFxListEx,")
    lines.append("    (u32)gGaxFxWaveSetEx,")
    for w in pkg_words[6:]:
        lines.append(f"    0x{w:08X},")
    lines.append("};")
    lines.append("")


def emit(music: list[dict], voice: list[dict], out_c: Path, out_h: Path) -> None:
    out_c.parent.mkdir(parents=True, exist_ok=True)
    # Counts are table lengths (max_id+1), not clip counts — sparse ids
    # (e.g. only id 1 after deleting id 0) must still pass id < count checks.
    music_count = (
        max((parse_int(m.get("id", 0)) for m in music), default=-1) + 1 if music else 0
    )
    voice_count = (
        max((v["_id"] for v in voice), default=-1) + 1 if voice else 0
    )
    lines_h = [
        "#ifndef GUARD_GAX_CATALOG_H",
        "#define GUARD_GAX_CATALOG_H",
        "",
        "#include \"gba/types.h\"",
        "#include \"gax.h\"",
        "",
        f"#define GAX_MUSIC_COUNT {music_count}",
        f"#define GAX_VOICE_COUNT {voice_count}",
        "",
    ]
    for m in music:
        lines_h.append(f"#define GAX_MUSIC_{c_ident(m.get('name', str(m.get('id')))).upper()} {parse_int(m.get('id', 0))}")
    for v in voice:
        lines_h.append(f"#define {v['_constant']} {v['_id']}")
    lines_h += [
        "",
        "extern const u16 gGaxMusicCount;",
        "extern const u16 gGaxVoiceCount;",
        "extern const void *const gGaxMusicTable[];",
        "extern const GaxSpeechEntry gGaxVoiceTable[];",
        "",
        f"/* FX voice: gax_fx id for clip N = GAX_VOICE_FX_BASE + N. */",
        f"#define GAX_VOICE_FX_BASE {GAX_FX_LIST_COUNT}",
        f"#define GAX_VOICE_FX_NOTE {VOICE_FX_NOTE}",
        "#define GAX_VOICE_FX_PRIORITY 0x7FFF",
        "extern const u32 gGaxPackageEx[];",
        "",
        "#endif /* GUARD_GAX_CATALOG_H */",
        "",
    ]
    out_h.write_text("\n".join(lines_h))

    lines = [
        "/* Auto-generated by tools/build_gax_catalog.py — do not edit. */",
        "#include \"runtime.h\"",
        "#include \"gax_catalog.h\"",
        "",
        f"APPEND_RODATA const u16 gGaxMusicCount = {music_count};",
        f"APPEND_RODATA const u16 gGaxVoiceCount = {voice_count};",
        "",
    ]

    # Music: vanilla pointers or packed blobs as APPEND_RODATA
    max_music_id = max((parse_int(m.get("id", 0)) for m in music), default=-1)
    music_slots = [None] * (max_music_id + 1 if max_music_id >= 0 else 0)
    for m in music:
        music_slots[parse_int(m.get("id", 0))] = m

    for m in music:
        mid = parse_int(m.get("id", 0))
        name = c_ident(m.get("name", f"music_{mid}"))
        if m.get("vanilla_module"):
            addr = parse_int(m["vanilla_module"])
            lines.append(f"/* music {mid} {name}: vanilla module */")
            lines.append(f"#define GAX_MUSIC_PTR_{mid} ((const void *)0x{addr:08X})")
        else:
            # pack song
            packed = m["_path"].parent / "built" / f"{m['_path'].stem}.bin"
            packed.parent.mkdir(parents=True, exist_ok=True)
            subprocess.check_call(
                [
                    sys.executable,
                    str(PACK_SONG),
                    str(m["_path"]),
                    "-o",
                    str(packed),
                ]
            )
            if packed.suffix == ".bin" and packed.exists() and packed.stat().st_size > 8:
                data = packed.read_bytes()
                lines.append(f"APPEND_RODATA static const u8 sMusicBlob_{mid}[] = {{")
                for i in range(0, len(data), 16):
                    chunk = ", ".join(f"0x{b:02X}" for b in data[i : i + 16])
                    lines.append(f"    {chunk},")
                lines.append("};")
                lines.append(f"#define GAX_MUSIC_PTR_{mid} ((const void *)sMusicBlob_{mid})")
            else:
                # vanilla_module sidecar from packer skip
                side = json.loads(packed.read_text()) if packed.exists() else {}
                addr = parse_int(side.get("vanilla_module", 0x081310F0))
                lines.append(f"#define GAX_MUSIC_PTR_{mid} ((const void *)0x{addr:08X})")

    lines.append("")
    lines.append("APPEND_RODATA const void *const gGaxMusicTable[] = {")
    if not music_slots:
        lines.append("    0,")
    for i, m in enumerate(music_slots):
        if m is None:
            lines.append("    0,")
        else:
            lines.append(f"    GAX_MUSIC_PTR_{parse_int(m.get('id', i))},")
    lines.append("};")
    lines.append("")

    # Voice blobs
    max_voice_id = max((v["_id"] for v in voice), default=-1)
    voice_slots = [None] * (max_voice_id + 1 if max_voice_id >= 0 else 0)
    for v in voice:
        voice_slots[v["_id"]] = v

    for v in voice:
        vid = v["_id"]
        data = Path(v["_bin"]).read_bytes()
        lines.append(f"APPEND_RODATA static const u8 sVoiceBlob_{vid}[] = {{")
        for i in range(0, len(data), 16):
            chunk = ", ".join(f"0x{b:02X}" for b in data[i : i + 16])
            lines.append(f"    {chunk},")
        lines.append("};")

    lines.append("")
    lines.append("APPEND_RODATA const GaxSpeechEntry gGaxVoiceTable[] = {")
    if not voice_slots:
        lines.append("    {0, 0},")
    for i, v in enumerate(voice_slots):
        if v is None:
            lines.append("    {0, 0},")
        else:
            vid = v["_id"]
            sf = v["_entry"]["size_flags"]
            lines.append(f"    {{ sVoiceBlob_{vid}, 0x{sf:08X} }},")
    lines.append("};")
    lines.append("")

    emit_voice_fx(lines, voice_slots)

    out_c.write_text("\n".join(lines))
    print(f"wrote {out_c} ({len(music)} music, {len(voice)} voice)")
    write_voice_inventory(voice)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--out-c", type=Path, default=OUT_DIR / "gax_catalog.c")
    ap.add_argument("--out-h", type=Path, default=OUT_H_DIR / "gax_catalog.h")
    args = ap.parse_args()
    music = collect_music()
    voice = collect_voice()
    emit(music, voice, args.out_c, args.out_h)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
