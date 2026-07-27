#!/usr/bin/env python3
"""Pack a minimal GAX song module from JSON + WAV samples.

Song IR (sound/music/<name>.json):
{
  "id": 0,
  "name": "test_bgm",
  "channels": 1,
  "rows_per_pattern": 32,
  "patterns_per_channel": 1,
  "loop_point": 0,
  "master_volume": 256,
  "mixing_rate": 13380,
  "instruments": [{"sample": "samples/beep.wav", "loop": false}],
  "patterns": [
    {"channel": 0, "notes": [{"row": 0, "note": 60, "instrument": 0}]}
  ]
}

For a first slice this emits a relocatable module that reuses the vanilla
shared instrument/sample banks from Sigma Star Saga when "vanilla_template"
is set, or builds a tiny one-shot PCM song from local WAVs.

When "vanilla_module" is set to a ROM address, the catalog just references
that pointer (no pack) — useful for demos.
"""

from __future__ import annotations

import argparse
import json
import struct
import wave
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]


def read_wav_s8(path: Path) -> bytes:
    with wave.open(str(path), "rb") as wf:
        ch = wf.getnchannels()
        sw = wf.getsampwidth()
        raw = wf.readframes(wf.getnframes())
    if sw == 2:
        samples = list(struct.unpack("<" + "h" * (len(raw) // 2), raw))
        if ch == 2:
            samples = [
                (samples[i] + samples[i + 1]) // 2
                for i in range(0, len(samples) - 1, 2)
            ]
        return bytes((s >> 8) & 0xFF for s in samples)
    if sw == 1:
        data = bytes((b - 128) & 0xFF for b in raw)
        if ch == 2:
            data = bytes(
                ((data[i] + data[i + 1]) // 2) & 0xFF
                for i in range(0, len(data) - 1, 2)
            )
        return data
    raise ValueError(f"bad wav {path}")


def pack_simple(meta: dict, base_dir: Path, out_bin: Path) -> None:
    """Emit a standalone GAX-like song header + one PCM sample.

    This is a relocatable blob whose header pointers are absolute VMAs filled
    in later by build_gax_catalog.py (placeholder 0 for now; catalog patches).
    """
    instruments = meta.get("instruments") or []
    if not instruments:
        raise ValueError("need at least one instrument/sample")

    sample_path = base_dir / instruments[0]["sample"]
    pcm = read_wav_s8(sample_path)
    if len(pcm) < 16:
        pcm = pcm + b"\x00" * (16 - len(pcm))

    # Layout:
    # 0x00 song header (0x40 + channel pattern ptrs)
    # then sample data, sample entries, instrument stubs, pattern bytes
    channels = int(meta.get("channels", 1))
    rows = int(meta.get("rows_per_pattern", 32))
    patterns = int(meta.get("patterns_per_channel", 1))
    loop = int(meta.get("loop_point", 0))
    master = int(meta.get("master_volume", 0x100))
    mix = int(meta.get("mixing_rate", 13380))

    header_size = 0x28 + 4 * channels
    # We'll assemble with relative offsets, then note them in sidecar JSON.
    parts: list[bytes] = []
    cursor = 0

    def align4(n: int) -> int:
        return (n + 3) & ~3

    sample_off = align4(header_size)
    sample_entries_off = align4(sample_off + len(pcm))
    # sample entries: [start_marker, end_marker, sample0...]
    # entry = {addr:u32, size:u32}; first two are pool bounds in some titles
    sample_entry_bytes = 8 * 3
    instr_off = align4(sample_entries_off + sample_entry_bytes)
    # one instrument header stub (minimal)
    instr_bytes = 0x20
    seq_off = align4(instr_off + instr_bytes)
    # empty-ish sequence with end markers
    seq = bytearray(rows * 2)
    for i in range(rows):
        seq[i * 2] = 0xFF  # empty row marker commonly seen
        seq[i * 2 + 1] = 0x00
    # apply notes from pattern 0
    for pat in meta.get("patterns") or []:
        if int(pat.get("channel", 0)) != 0:
            continue
        for note in pat.get("notes") or []:
            row = int(note["row"])
            if 0 <= row < rows:
                # store MIDI note in low byte as a soft hint (engine-specific)
                seq[row * 2] = int(note.get("note", 60)) & 0x7F
                seq[row * 2 + 1] = int(note.get("instrument", 0)) & 0xFF
    seq_bytes = bytes(seq)
    pattern_table_off = align4(seq_off + len(seq_bytes))
    # pattern table entry: offset:u16, transpose:i8, reserved:u8
    pat_table = struct.pack("<Hbb", 0, 0, 0) * patterns

    total = pattern_table_off + len(pat_table) + 4 * channels
    blob = bytearray(align4(total + header_size))

    # Fill sample + tables at absolute offsets within blob
    blob[sample_off : sample_off + len(pcm)] = pcm
    # sample entries (pool start, pool end, sample)
    se = sample_entries_off
    struct.pack_into("<II", blob, se, sample_off, 0)
    struct.pack_into("<II", blob, se + 8, sample_off + len(pcm), 0)
    struct.pack_into("<II", blob, se + 16, sample_off, len(pcm))

    blob[instr_off : instr_off + 4] = struct.pack("<I", instr_off)  # self stub
    blob[seq_off : seq_off + len(seq_bytes)] = seq_bytes
    blob[pattern_table_off : pattern_table_off + len(pat_table)] = pat_table

    # Song header
    struct.pack_into(
        "<HHHHHH",
        blob,
        0,
        channels,
        rows,
        patterns,
        loop,
        master,
        0,
    )
    # seq ptr, instr set ptr, sample set ptr — filled with in-blob offsets;
    # catalog relocates by adding VMA base.
    struct.pack_into("<III", blob, 0x0C, seq_off, instr_off, sample_entries_off)
    struct.pack_into("<HHBBH", blob, 0x18, mix, 0, 4, 0, 0)
    # pattern table pointers per channel (offsets)
    for ch in range(channels):
        struct.pack_into("<I", blob, 0x28 + ch * 4, pattern_table_off)

    out_bin.write_bytes(bytes(blob))
    meta_out = {
        "blob_bytes": len(blob),
        "sample_off": sample_off,
        "seq_off": seq_off,
        "needs_relocation": True,
        "note": "Best-effort minimal module; prefer vanilla_module for reliable BGM.",
    }
    out_bin.with_suffix(".meta.json").write_text(json.dumps(meta_out, indent=2) + "\n")
    print(f"wrote {out_bin} ({len(blob)} bytes)")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("json", type=Path)
    ap.add_argument("-o", "--out", type=Path, required=True)
    args = ap.parse_args()
    meta = json.loads(args.json.read_text())
    if meta.get("vanilla_module"):
        print(f"skip pack — vanilla_module={meta['vanilla_module']}")
        args.out.write_text(
            json.dumps({"vanilla_module": meta["vanilla_module"]}) + "\n"
        )
        return 0
    pack_simple(meta, args.json.parent, args.out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
