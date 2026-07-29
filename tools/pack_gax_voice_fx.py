#!/usr/bin/env python3
"""Convert a voice clip (WAV/MP3/…) to GAX FX DPCM sample data.

Pipeline: pitch+trim to unsigned 8-bit PCM @ FX mix rate (15769 Hz), then
pack signed 4-bit delta DPCM with per-clip step (q*step) so speech slopes
fit without heavy clamp muffling (~2× vs u8). Perf note 0x3A (speed-tuned).

Emits:
  <out>            — .fx.dpcm blob (VD1 header + packed nibbles)
  <out>.entry.json — {rate, samples, bytes, encoding: dpcm4_u8, dpcm_step, …}
"""

from __future__ import annotations

import argparse
import json
import struct
import subprocess
import sys
from pathlib import Path

from pack_gax_speech import find_ffmpeg

FX_MIX_RATE = 15769
FX_NATURAL_NOTE = 0x3A  # speed-tuned; see build_gax_catalog.py
# Default pitch-only shift (semitones). Negative = lower; does not change tempo.
DEFAULT_PITCH_SEMITONES = -5
# |sample − 0x80| ≤ threshold counts as near-silence for end trimming.
SILENCE_THRESHOLD = 8
# Must match GAX_VOICE_DECODE_BYTES / gVoiceDecodeBuf in ram_map.
GAX_VOICE_DECODE_BYTES = 0x14000
DPCM_MAGIC = b"VD1\0"
_WORK_RATE = 44100


def _atempo_chain(factor: float) -> list[str]:
    """Split an atempo factor into ffmpeg-legal 0.5..2.0 stages."""
    filters: list[str] = []
    f = factor
    while f < 0.5:
        filters.append("atempo=0.5")
        f /= 0.5
    while f > 2.0:
        filters.append("atempo=2.0")
        f /= 2.0
    if abs(f - 1.0) > 1e-6:
        filters.append(f"atempo={f:.8f}")
    return filters


def _pitch_filters(semitones: float) -> list[str]:
    """Pitch-shift without tempo change, then resample to FX_MIX_RATE."""
    if abs(semitones) < 1e-9:
        return [f"aresample={FX_MIX_RATE}"]
    ratio = 2.0 ** (semitones / 12.0)
    # asetrate changes pitch+speed; atempo=1/ratio restores duration.
    parts = [
        f"aresample={_WORK_RATE}",
        f"asetrate={_WORK_RATE * ratio:.8f}",
        f"aresample={_WORK_RATE}",
        *_atempo_chain(1.0 / ratio),
        f"aresample={FX_MIX_RATE}",
    ]
    return parts


def trim_near_silence(pcm: bytes, threshold: int = SILENCE_THRESHOLD) -> tuple[bytes, int, int]:
    """Drop leading/trailing samples with |s−0x80| ≤ threshold. Keeps ≥1 byte."""
    if not pcm:
        return pcm, 0, 0

    def quiet(b: int) -> bool:
        return abs(b - 0x80) <= threshold

    start = 0
    while start < len(pcm) and quiet(pcm[start]):
        start += 1
    end = len(pcm)
    while end > start and quiet(pcm[end - 1]):
        end -= 1
    if start >= end:
        # All quiet — keep a tiny silence pad so the wave is non-empty.
        return bytes([0x80]), len(pcm) - 1, 0
    return pcm[start:end], start, len(pcm) - end


def _clamp_u8(x: int) -> int:
    if x < 0:
        return 0
    if x > 255:
        return 255
    return x


def _trial_encode(pcm: bytes, step: int) -> tuple[bytes, int]:
    """Encode with step; return (decoded_pcm, clamp_count)."""
    first = pcm[0]
    pred = first
    out = bytearray(len(pcm))
    out[0] = first
    clamps = 0
    for i in range(1, len(pcm)):
        delta = int(pcm[i]) - pred
        q = int(round(delta / step))
        if q < -8:
            q = -8
            clamps += 1
        elif q > 7:
            q = 7
            clamps += 1
        pred = _clamp_u8(pred + q * step)
        out[i] = pred
    return bytes(out), clamps


def choose_dpcm_step(pcm: bytes) -> int:
    """Pick step 1..16 minimizing MAE (4-bit deltas = q*step)."""
    if len(pcm) < 2:
        return 1
    best_step = 1
    best_mae = 1e30
    for step in range(1, 17):
        dec, _clamps = _trial_encode(pcm, step)
        mae = sum(abs(a - b) for a, b in zip(pcm, dec)) / len(pcm)
        if mae < best_mae:
            best_mae = mae
            best_step = step
    return best_step


def encode_dpcm4_u8(pcm: bytes, step: int | None = None) -> bytes:
    """Pack unsigned PCM into VD1 + signed 4-bit delta body (high nibble first).

    Header: magic, sampleCount, firstSample, step, pad[2].
    Each nibble q in [-8,7] advances the predictor by q*step (step≥1).
    """
    if not pcm:
        raise ValueError("empty PCM")
    if len(pcm) > GAX_VOICE_DECODE_BYTES:
        raise ValueError(
            f"sampleCount {len(pcm)} exceeds GAX_VOICE_DECODE_BYTES "
            f"({GAX_VOICE_DECODE_BYTES}); shorten the clip or raise the buffer"
        )
    if step is None:
        step = choose_dpcm_step(pcm)
    if step < 1:
        step = 1
    if step > 255:
        step = 255
    first = pcm[0]
    pred = first
    nibbles: list[int] = []
    for s in pcm[1:]:
        delta = int(s) - pred
        q = int(round(delta / step))
        if q < -8:
            q = -8
        elif q > 7:
            q = 7
        nibbles.append(q & 0xF)
        pred = _clamp_u8(pred + q * step)
    body = bytearray((len(nibbles) + 1) // 2)
    for i, n in enumerate(nibbles):
        if (i & 1) == 0:
            body[i // 2] = (n & 0xF) << 4
        else:
            body[i // 2] |= n & 0xF
    header = DPCM_MAGIC + struct.pack("<IBB2x", len(pcm), first, step)
    return header + bytes(body)


def decode_dpcm4_u8(blob: bytes, dst_cap: int = GAX_VOICE_DECODE_BYTES) -> bytes:
    """Host-side round-trip helper (mirrors runtime decoder)."""
    if len(blob) < 12 or blob[:4] != DPCM_MAGIC:
        raise ValueError("bad DPCM header")
    sample_count, first, step = struct.unpack_from("<IBB", blob, 4)
    if step == 0:
        step = 1
    if sample_count == 0 or sample_count > dst_cap:
        raise ValueError(f"bad sampleCount {sample_count}")
    out = bytearray(sample_count)
    out[0] = first
    pred = first
    body = blob[12:]
    need = sample_count - 1
    for i in range(need):
        bi = i // 2
        if bi >= len(body):
            raise ValueError("truncated DPCM body")
        if (i & 1) == 0:
            n = (body[bi] >> 4) & 0xF
        else:
            n = body[bi] & 0xF
        if n >= 8:
            q = n - 16
        else:
            q = n
        pred = _clamp_u8(pred + q * step)
        out[i + 1] = pred
    return bytes(out)


def pack(audio: Path, out: Path, pitch_semitones: float = DEFAULT_PITCH_SEMITONES) -> None:
    ffmpeg = find_ffmpeg()
    out.parent.mkdir(parents=True, exist_ok=True)
    af = ",".join(_pitch_filters(pitch_semitones))
    tmp = out.with_suffix(out.suffix + ".raw")
    cmd = [
        ffmpeg,
        "-y",
        "-i",
        str(audio),
        "-ac",
        "1",
        "-af",
        af,
        "-ar",
        str(FX_MIX_RATE),
        "-f",
        "u8",
        "-acodec",
        "pcm_u8",
        str(tmp),
    ]
    subprocess.check_call(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    raw = tmp.read_bytes()
    tmp.unlink(missing_ok=True)
    pcm, trim_lead, trim_trail = trim_near_silence(raw)
    if len(pcm) > GAX_VOICE_DECODE_BYTES:
        print(
            f"warning: clamping {len(pcm)} samples to {GAX_VOICE_DECODE_BYTES}",
            file=sys.stderr,
        )
        pcm = pcm[:GAX_VOICE_DECODE_BYTES]
    dpcm = encode_dpcm4_u8(pcm)
    out.write_bytes(dpcm)
    n = len(pcm)
    step = dpcm[9] or 1
    meta = {
        "source": str(audio),
        "rate": FX_MIX_RATE,
        "samples": n,
        "bytes": len(dpcm),
        "pcm_bytes": n,
        "raw_samples": len(raw),
        "trim_lead": trim_lead,
        "trim_trail": trim_trail,
        "silence_threshold": SILENCE_THRESHOLD,
        "note": FX_NATURAL_NOTE,
        "pitch_semitones": pitch_semitones,
        "encoding": "dpcm4_u8",
        "dpcm_step": step,
        "first_sample": pcm[0],
    }
    out.with_suffix(out.suffix + ".entry.json").write_text(
        json.dumps(meta, indent=2) + "\n"
    )
    print(
        f"wrote {out} ({len(dpcm)} bytes DPCM / {n} samples @ {FX_MIX_RATE} Hz, "
        f"step={step}, pitch {pitch_semitones:+.1f} st, "
        f"trimmed {trim_lead}+{trim_trail} of {len(raw)})"
    )


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("audio", type=Path)
    ap.add_argument("-o", "--out", type=Path, required=True)
    ap.add_argument(
        "--pitch-semitones",
        type=float,
        default=DEFAULT_PITCH_SEMITONES,
        help="pitch shift in semitones without changing duration (default: %(default)s)",
    )
    args = ap.parse_args()
    pack(args.audio, args.out, pitch_semitones=args.pitch_semitones)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
