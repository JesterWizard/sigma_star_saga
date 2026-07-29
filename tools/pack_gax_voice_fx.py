#!/usr/bin/env python3
"""Convert a voice clip (WAV/MP3/…) to GAX FX sample data.

GAX FX samples are unsigned 8-bit PCM centered at 0x80 (silence), same as
vanilla FX wave-set payloads. Encoded at the FX mixing rate (15769 Hz) with
perf note 0x3A (speed-tuned). Optional pitch_semitones shifts pitch without
changing duration (asetrate + aresample + atempo), since FX note locks
speed and pitch together.

Emits:
  <out>            — raw unsigned 8-bit PCM (silence = 0x80)
  <out>.entry.json — {rate, samples, bytes, note, pitch_semitones}
"""

from __future__ import annotations

import argparse
import json
import math
import subprocess
from pathlib import Path

from pack_gax_speech import find_ffmpeg

FX_MIX_RATE = 15769
FX_NATURAL_NOTE = 0x3A  # speed-tuned; see build_gax_catalog.py
# Default pitch-only shift (semitones). Negative = lower; does not change tempo.
DEFAULT_PITCH_SEMITONES = -5
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


def pack(audio: Path, out: Path, pitch_semitones: float = DEFAULT_PITCH_SEMITONES) -> None:
    ffmpeg = find_ffmpeg()
    out.parent.mkdir(parents=True, exist_ok=True)
    af = ",".join(_pitch_filters(pitch_semitones))
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
        str(out),
    ]
    subprocess.check_call(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    n = out.stat().st_size
    meta = {
        "source": str(audio),
        "rate": FX_MIX_RATE,
        "samples": n,
        "bytes": n,
        "note": FX_NATURAL_NOTE,
        "pitch_semitones": pitch_semitones,
        "encoding": "pcm_u8_fx",
    }
    out.with_suffix(out.suffix + ".entry.json").write_text(
        json.dumps(meta, indent=2) + "\n"
    )
    print(
        f"wrote {out} ({n} bytes u8 @ {FX_MIX_RATE} Hz, "
        f"pitch {pitch_semitones:+.1f} st)"
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
