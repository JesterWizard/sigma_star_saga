#!/usr/bin/env python3
"""Pack mono audio (WAV / MP3 / …) into a GAX_SPEECH bitstream blob.

Non-WAV inputs are converted with ffmpeg to mono 16-bit PCM first:
  ffmpeg -y -i <src> -ac 1 -ar 13379 -sample_fmt s16 <tmp.wav>

Layout (see documentation/gax-audio.md):
  [0, bit_offset): 33-byte frames (high nibble 0xD)
  [bit_offset, end): 1 bit per mixer tick (all 1s = decode every tick)

Emits:
  <out_stem>.bin          — raw speech payload
  <out_stem>.entry.json   — {bit_offset, size_flags, bytes}
"""

from __future__ import annotations

import argparse
import json
import os
import shutil
import struct
import subprocess
import wave
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
FRAME = 0x21  # 33
DST_RATE = 13379


def find_ffmpeg() -> str:
    env = os.environ.get("FFMPEG")
    candidates = []
    if env:
        candidates.append(env)
    candidates.append(str(REPO / "tools" / "bin" / "ffmpeg"))
    which = shutil.which("ffmpeg")
    if which:
        candidates.append(which)
    for c in candidates:
        if c and Path(c).is_file() and os.access(c, os.X_OK):
            return c
    raise FileNotFoundError(
        "ffmpeg not found (needed for non-WAV voice sources). "
        "Install ffmpeg, put a binary at tools/bin/ffmpeg, or set FFMPEG=."
    )


def convert_to_wav(src: Path, wav_out: Path) -> None:
    ffmpeg = find_ffmpeg()
    wav_out.parent.mkdir(parents=True, exist_ok=True)
    cmd = [
        ffmpeg,
        "-y",
        "-i",
        str(src),
        "-ac",
        "1",
        "-ar",
        str(DST_RATE),
        "-sample_fmt",
        "s16",
        str(wav_out),
    ]
    subprocess.check_call(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    print(f"ffmpeg: {src.name} → {wav_out.name} ({DST_RATE} Hz mono)")


def resolve_wav(audio: Path, out_stem: Path) -> tuple[Path, Path | None]:
    """Return (wav_path, temp_to_delete_or_None)."""
    if audio.suffix.lower() == ".wav":
        return audio, None
    tmp = out_stem.with_suffix(".tmp.wav")
    convert_to_wav(audio, tmp)
    return tmp, tmp


def read_wav_mono_s16(path: Path) -> tuple[list[int], int]:
    with wave.open(str(path), "rb") as wf:
        ch = wf.getnchannels()
        sw = wf.getsampwidth()
        rate = wf.getframerate()
        n = wf.getnframes()
        raw = wf.readframes(n)
    if sw == 2:
        samples = list(struct.unpack("<" + "h" * (len(raw) // 2), raw))
    elif sw == 1:
        samples = [(b - 128) * 256 for b in raw]
    else:
        raise ValueError(f"unsupported sample width {sw}")
    if ch == 2:
        samples = [
            (samples[i] + samples[i + 1]) // 2 for i in range(0, len(samples) - 1, 2)
        ]
    elif ch != 1:
        raise ValueError(f"unsupported channels {ch}")
    return samples, rate


def pcm_to_frames(samples: list[int], src_rate: int, dst_rate: int = DST_RATE) -> bytes:
    """Best-effort frames: 0xD0 header + 32 signed bytes of downsampled PCM."""
    if not samples:
        samples = [0]
    ratio = src_rate / float(dst_rate)
    out = bytearray()
    # ~one frame per 32 output samples
    n_out = max(1, int(len(samples) / ratio / 32))
    for fi in range(n_out):
        frame = bytearray(FRAME)
        frame[0] = 0xD0
        for i in range(32):
            src_i = int((fi * 32 + i) * ratio)
            if src_i >= len(samples):
                src_i = len(samples) - 1
            # fold s16 → s8
            frame[1 + i] = (samples[src_i] >> 8) & 0xFF
        out.extend(frame)
    return bytes(out)


def build_bitstream(n_bits: int) -> bytes:
    n_bytes = (n_bits + 7) // 8
    # all ones → fire a frame decode attempt every mixer tick
    return b"\xff" * n_bytes


def pack(audio: Path, out_stem: Path) -> None:
    wav, tmp = resolve_wav(audio, out_stem)
    try:
        samples, rate = read_wav_mono_s16(wav)
        frames = pcm_to_frames(samples, rate)
        n_frames = len(frames) // FRAME
        # bits ≈ frames * 8 (over-provision so playback isn't starved)
        n_bits = max(n_frames * 8, 32)
        bits = build_bitstream(n_bits)
        bit_offset = len(frames)
        payload = frames + bits
        size_flags = 0x80000000 | (bit_offset & 0x1FFFFFFF)

        out_stem.parent.mkdir(parents=True, exist_ok=True)
        bin_path = out_stem.with_suffix(".bin")
        bin_path.write_bytes(payload)
        meta = {
            "source": str(audio),
            "bit_offset": bit_offset,
            "size_flags": size_flags,
            "bytes": len(payload),
            "frames": n_frames,
        }
        out_stem.with_suffix(".entry.json").write_text(json.dumps(meta, indent=2) + "\n")
        print(f"wrote {bin_path} ({len(payload)} bytes, {n_frames} frames)")
    finally:
        if tmp is not None:
            tmp.unlink(missing_ok=True)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument(
        "audio",
        type=Path,
        help="input WAV, MP3, or other format ffmpeg can decode",
    )
    ap.add_argument("-o", "--out", type=Path, required=True, help="output stem")
    args = ap.parse_args()
    pack(args.audio, args.out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
