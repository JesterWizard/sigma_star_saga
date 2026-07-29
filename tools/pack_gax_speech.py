#!/usr/bin/env python3
"""Pack mono audio (WAV / MP3 / …) into a GAX_SPEECH bitstream blob.

Non-WAV inputs are converted with ffmpeg to mono 16-bit PCM first:
  ffmpeg -y -i <src> -ac 1 -ar 2400 -sample_fmt s16 <tmp.wav>

Decoder contract (vanilla @ 0x0805708C / consumer @ 0x08056A30):
  - Each frame is exactly 0x21 bytes; byte0 high nibble must be 0xD
  - Remaining 260 bits unpack MSB-first into 76 halfwords (see FIELD_TABLE)
  - Gate bitstream at bit_offset: 1 = decode this tick, 0 = skip
  - Auto-stop when frame cursor reaches bit_offset
  - 40 PCM samples/frame at ~2400 Hz (one frame per 60 Hz mixer tick; ROM init stores 0x28)

This packer analyses PCM (voiced/unvoiced, pitch, energy, band shape) and
quantizes into the recovered field table. Silence remains a valid all-zero
coefficient vector under header 0xD.

Emits:
  <out_stem>.bin          — raw speech payload
  <out_stem>.entry.json   — {bit_offset, size_flags, bytes, …}
"""

from __future__ import annotations

import argparse
import json
import math
import os
import shutil
import struct
import subprocess
import wave
from dataclasses import dataclass
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
FRAME = 0x21  # 33
DST_RATE = 2400  # engine consumes one 40-sample frame per 60 Hz mixer tick
SAMPLES_PER_FRAME = 40
PAYLOAD_BITS = 260
HEADER_NIBBLE = 0xD

# Ordered MSB-first after the 0xD nibble. Offsets relative to speechState+4.
# Widths sum to exactly 260; every decode halfword store is represented.
FIELD_TABLE: list[tuple[int, int, str]] = [
    (0x3F8, 6, "pitch_q"),
    (0x3FA, 6, "energy_q"),
    (0x3FC, 5, "voiced_mix"),
    (0x3FE, 5, "tilt"),
    (0x400, 4, "band0"),
    (0x402, 4, "band1"),
    (0x404, 3, "band2"),
    (0x406, 3, "band3"),
    (0x408, 7, "lpc0"),
    (0x418, 2, "lpc0b"),
    (0x410, 2, "lpc1b"),
    (0x420, 6, "lpc1"),
    (0x428, 3, "c00"),
    (0x42A, 3, "c01"),
    (0x42C, 3, "c02"),
    (0x42E, 3, "c03"),
    (0x430, 3, "c04"),
    (0x432, 3, "c05"),
    (0x434, 3, "c06"),
    (0x436, 3, "c07"),
    (0x438, 3, "c08"),
    (0x43A, 3, "c09"),
    (0x43C, 3, "c0a"),
    (0x43E, 3, "c0b"),
    (0x440, 3, "c0c"),
    (0x40A, 7, "lpc2"),
    (0x41A, 2, "lpc2b"),
    (0x412, 2, "lpc3b"),
    (0x422, 6, "lpc3"),
    (0x442, 3, "c10"),
    (0x444, 3, "c11"),
    (0x446, 3, "c12"),
    (0x448, 3, "c13"),
    (0x44A, 3, "c14"),
    (0x44C, 3, "c15"),
    (0x44E, 3, "c16"),
    (0x450, 3, "c17"),
    (0x452, 3, "c18"),
    (0x454, 3, "c19"),
    (0x456, 3, "c1a"),
    (0x458, 3, "c1b"),
    (0x45A, 3, "c1c"),
    (0x40C, 7, "lpc4"),
    (0x41C, 2, "lpc4b"),
    (0x414, 2, "lpc5b"),
    (0x424, 6, "lpc5"),
    (0x45C, 3, "c20"),
    (0x45E, 3, "c21"),
    (0x460, 3, "c22"),
    (0x462, 3, "c23"),
    (0x464, 3, "c24"),
    (0x466, 3, "c25"),
    (0x468, 3, "c26"),
    (0x46A, 3, "c27"),
    (0x46C, 3, "c28"),
    (0x46E, 3, "c29"),
    (0x470, 3, "c2a"),
    (0x472, 3, "c2b"),
    (0x474, 3, "c2c"),
    (0x40E, 7, "lpc6"),
    (0x41E, 2, "lpc6b"),
    (0x416, 2, "lpc7b"),
    (0x426, 6, "lpc7"),
    (0x476, 3, "c30"),
    (0x478, 3, "c31"),
    (0x47A, 3, "c32"),
    (0x47C, 3, "c33"),
    (0x47E, 3, "c34"),
    (0x480, 3, "c35"),
    (0x482, 3, "c36"),
    (0x484, 3, "c37"),
    (0x486, 3, "c38"),
    (0x488, 3, "c39"),
    (0x48A, 3, "c3a"),
    (0x48C, 3, "c3b"),
    (0x48E, 3, "c3c"),
]

assert sum(w for _, w, _ in FIELD_TABLE) == PAYLOAD_BITS
assert len(FIELD_TABLE) == 76
assert len({o for o, _, _ in FIELD_TABLE}) == 76


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


def resample_linear(samples: list[int], src_rate: int, dst_rate: int) -> list[int]:
    if src_rate == dst_rate:
        return list(samples)
    if not samples:
        return [0]
    ratio = src_rate / float(dst_rate)
    n_out = max(1, int(len(samples) / ratio))
    out: list[int] = []
    for i in range(n_out):
        src = i * ratio
        i0 = int(src)
        i1 = min(i0 + 1, len(samples) - 1)
        frac = src - i0
        out.append(int(samples[i0] * (1.0 - frac) + samples[i1] * frac))
    return out


def frame_nibble_ok(frame: bytes) -> bool:
    return len(frame) >= 1 and (frame[0] >> 4) == HEADER_NIBBLE


def _clamp_u(value: int, width: int) -> int:
    return max(0, min(value, (1 << width) - 1))


def pack_fields(fields: dict[int, int]) -> bytes:
    """Pack field dict {offset: value} into a 0x21-byte frame."""
    bits: list[int] = []
    for _ in range(4):
        bits.append((HEADER_NIBBLE >> (3 - _)) & 1)
    for off, width, _name in FIELD_TABLE:
        value = _clamp_u(int(fields.get(off, 0)), width)
        for b in range(width - 1, -1, -1):
            bits.append((value >> b) & 1)
    assert len(bits) == 4 + PAYLOAD_BITS
    out = bytearray(FRAME)
    for i, bit in enumerate(bits):
        if bit:
            out[i >> 3] |= 1 << (7 - (i & 7))
    # Force header nibble (in case of drift).
    out[0] = (out[0] & 0x0F) | (HEADER_NIBBLE << 4)
    return bytes(out)


def unpack_fields(frame: bytes) -> dict[int, int] | None:
    """Python mirror of GaxSpeechDecodeUnpack; None if header reject."""
    if not frame_nibble_ok(frame) or len(frame) < FRAME:
        return None
    bit_pos = 4
    out: dict[int, int] = {}
    for off, width, _name in FIELD_TABLE:
        value = 0
        for _ in range(width):
            src_bit = 7 - (bit_pos & 7)
            bit = (frame[bit_pos >> 3] >> src_bit) & 1
            value = (value << 1) | bit
            bit_pos += 1
        out[off] = value
    if bit_pos != 4 + PAYLOAD_BITS:
        return None
    return out


def unpack_header_fields(frame: bytes) -> dict[str, int] | None:
    """Back-compat helper used by older smoke tests."""
    fields = unpack_fields(frame)
    if fields is None:
        return None
    return {
        "+0x3F8": fields[0x3F8],
        "+0x3FA": fields[0x3FA],
        "+0x3FC": fields[0x3FC],
        "+0x3FE": fields[0x3FE],
    }


def make_silence_frame() -> bytes:
    return pack_fields({})


def verify_field_table() -> None:
    total = sum(w for _, w, _ in FIELD_TABLE)
    if total != PAYLOAD_BITS:
        raise RuntimeError(f"FIELD_TABLE bits {total} != {PAYLOAD_BITS}")
    offs = [o for o, _, _ in FIELD_TABLE]
    if len(set(offs)) != len(offs):
        raise RuntimeError("duplicate field offsets")
    # Round-trip random-ish values.
    probe = {o: (i * 3) & ((1 << w) - 1) for i, (o, w, _) in enumerate(FIELD_TABLE)}
    frame = pack_fields(probe)
    got = unpack_fields(frame)
    if got != probe:
        raise RuntimeError("pack/unpack round-trip mismatch")
    if unpack_fields(b"\xC0" + bytes(FRAME - 1)) is not None:
        raise RuntimeError("non-0xD header must reject")


@dataclass
class EncState:
    prev_pitch: float = 0.0
    prev_energy: float = 0.0
    phase: float = 0.0
    noise: int = 0x12345678


def _rms(frame: list[int]) -> float:
    if not frame:
        return 0.0
    acc = sum(s * s for s in frame) / float(len(frame))
    return math.sqrt(acc)


def _autocorr_pitch(frame: list[int], rate: int) -> tuple[bool, float]:
    """Return (voiced, period_samples)."""
    n = len(frame)
    if n < 16:
        return False, 0.0
    mean = sum(frame) / float(n)
    x = [s - mean for s in frame]
    energy = sum(v * v for v in x)
    if energy < 1.0:
        return False, 0.0
    min_p = max(2, int(rate / 400))
    max_p = min(n - 2, int(rate / 60))
    best_p = 0
    best_r = 0.0
    for p in range(min_p, max_p + 1):
        num = 0.0
        for i in range(n - p):
            num += x[i] * x[i + p]
        r = num / energy
        if r > best_r:
            best_r = r
            best_p = p
    voiced = best_r > 0.35 and best_p > 0
    return voiced, float(best_p)


def _band_energies(frame: list[int], bands: int = 8) -> list[float]:
    """Cheap DFT-magnitude band energies (log-ish)."""
    n = len(frame)
    if n == 0:
        return [0.0] * bands
    # Hann window
    windowed = [
        frame[i] * (0.5 - 0.5 * math.cos(2 * math.pi * i / max(1, n - 1)))
        for i in range(n)
    ]
    half = n // 2
    mags = []
    for k in range(1, half):
        re = 0.0
        im = 0.0
        for i, s in enumerate(windowed):
            ang = 2 * math.pi * k * i / n
            re += s * math.cos(ang)
            im -= s * math.sin(ang)
        mags.append(math.sqrt(re * re + im * im))
    if not mags:
        return [0.0] * bands
    out = []
    step = max(1, len(mags) // bands)
    for b in range(bands):
        chunk = mags[b * step : (b + 1) * step] or [0.0]
        out.append(sum(chunk) / len(chunk))
    return out


def _q(value: float, lo: float, hi: float, width: int) -> int:
    if hi <= lo:
        return 0
    t = (value - lo) / (hi - lo)
    t = 0.0 if t < 0.0 else 1.0 if t > 1.0 else t
    return _clamp_u(int(round(t * ((1 << width) - 1))), width)


def analyze_frame(frame: list[int], state: EncState) -> dict[int, int]:
    """Map one PCM frame into GAX vocoder fields with cross-frame smoothing."""
    energy = _rms(frame)
    voiced, period = _autocorr_pitch(frame, DST_RATE)
    bands = _band_energies(frame, 8)

    # Smooth energy / pitch to avoid clicks across frames.
    energy = 0.65 * energy + 0.35 * state.prev_energy
    if voiced and period > 0:
        if state.prev_pitch > 0:
            period = 0.7 * period + 0.3 * state.prev_pitch
        state.prev_pitch = period
    else:
        period = state.prev_pitch * 0.85 if state.prev_pitch else 0.0
        state.prev_pitch = period
    state.prev_energy = energy

    # Pitch quant: map period 60..400Hz → 6-bit. 0 = unvoiced/noise.
    if voiced and period > 1:
        hz = DST_RATE / period
        pitch_q = _q(hz, 60.0, 400.0, 6)
        if pitch_q == 0:
            pitch_q = 1
    else:
        pitch_q = 0

    energy_q = _q(math.log1p(energy), 0.0, math.log1p(20000.0), 6)
    voiced_mix = _q(1.0 if voiced else 0.15, 0.0, 1.0, 5)
    # Spectral tilt from high/low band ratio.
    low = sum(bands[:3]) + 1e-6
    high = sum(bands[5:]) + 1e-6
    tilt = _q(math.log(high / low), -3.0, 3.0, 5)

    fields: dict[int, int] = {
        0x3F8: pitch_q,
        0x3FA: energy_q,
        0x3FC: voiced_mix,
        0x3FE: tilt,
    }

    # First four short bands.
    for off, width, idx in (
        (0x400, 4, 0),
        (0x402, 4, 1),
        (0x404, 3, 2),
        (0x406, 3, 3),
    ):
        fields[off] = _q(math.log1p(bands[idx]), 0.0, math.log1p(max(bands) + 1), width)

    # Reflection-style coeffs: bias around mid code (ROM expand subtracts 0x20
    # on some paths). Keep mild shape from residual bands + energy.
    shape = []
    for i in range(8):
        shape.append(bands[i] / (sum(bands) + 1e-6))

    def mid_code(width: int, unit: float) -> int:
        mid = 1 << (width - 1)
        span = mid - 1
        return _clamp_u(int(round(mid + unit * span)), width)

    # Four LPC groups (7+2+2+6) + thirteen 3-bit coeffs each.
    lpc_groups = [
        (0x408, 0x418, 0x410, 0x420, list(range(0x428, 0x442, 2))),
        (0x40A, 0x41A, 0x412, 0x422, list(range(0x442, 0x45C, 2))),
        (0x40C, 0x41C, 0x414, 0x424, list(range(0x45C, 0x476, 2))),
        (0x40E, 0x41E, 0x416, 0x426, list(range(0x476, 0x490, 2))),
    ]
    for gi, (a, b, c, d, trips) in enumerate(lpc_groups):
        base = shape[gi % 8] - 0.125
        fields[a] = mid_code(7, base * 0.6)
        fields[b] = mid_code(2, base * 0.3)
        fields[c] = mid_code(2, -base * 0.2)
        fields[d] = mid_code(6, base * 0.45)
        for ti, off in enumerate(trips):
            unit = shape[(gi + ti) % 8] - 0.125
            # Silence / near-zero energy → exact mid/zero-ish codes stay quiet.
            if energy_q == 0:
                fields[off] = 0
            else:
                fields[off] = mid_code(3, unit * 0.8)

    if energy_q == 0:
        # Hard silence: keep header-valid zero vector (no static).
        return {o: 0 for o, _w, _n in FIELD_TABLE}

    # Ensure every table offset present.
    for off, width, _name in FIELD_TABLE:
        fields.setdefault(off, 0)
        fields[off] = _clamp_u(fields[off], width)
    return fields


def reference_synth_frame(fields: dict[int, int], state: EncState) -> list[int]:
    """Host-side approximate synthesizer for encoder self-check."""
    pitch_q = fields.get(0x3F8, 0)
    energy_q = fields.get(0x3FA, 0)
    voiced_mix = fields.get(0x3FC, 0) / 31.0
    amp = (energy_q / 63.0) ** 1.5 * 12000.0
    out: list[int] = []
    if amp < 1.0:
        return [0] * SAMPLES_PER_FRAME

    if pitch_q > 0:
        hz = 60.0 + (pitch_q / 63.0) * (400.0 - 60.0)
        phase_inc = 2 * math.pi * hz / DST_RATE
    else:
        phase_inc = 0.0

    # Mild formant from first band codes.
    bright = (fields.get(0x400, 0) + fields.get(0x402, 0)) / 30.0

    for _ in range(SAMPLES_PER_FRAME):
        state.noise = (state.noise * 1103515245 + 12345) & 0x7FFFFFFF
        noise = ((state.noise >> 16) - 16384) / 16384.0
        if phase_inc > 0:
            buzz = 1.0 if math.sin(state.phase) > 0 else -1.0
            state.phase = (state.phase + phase_inc) % (2 * math.pi)
        else:
            buzz = noise
        sample = amp * ((voiced_mix * buzz) + ((1.0 - voiced_mix) * noise))
        sample *= 0.7 + 0.3 * bright
        out.append(int(max(-32767, min(32767, sample))))
    return out


def pcm_compare(a: list[int], b: list[int]) -> float:
    n = min(len(a), len(b))
    if n == 0:
        return 0.0
    err = sum((a[i] - b[i]) * (a[i] - b[i]) for i in range(n)) / float(n)
    return math.sqrt(err)


def pcm_to_frames(
    samples: list[int], src_rate: int, dst_rate: int = DST_RATE
) -> tuple[bytes, list[int]]:
    """Analyse PCM into vocoder frames; also return reference synth PCM."""
    pcm = resample_linear(samples, src_rate, dst_rate)
    if not pcm:
        pcm = [0]
    # Pad to whole frames.
    rem = len(pcm) % SAMPLES_PER_FRAME
    if rem:
        pcm.extend([0] * (SAMPLES_PER_FRAME - rem))
    n_frames = max(1, len(pcm) // SAMPLES_PER_FRAME)

    enc = EncState()
    synth_state = EncState()
    blobs = bytearray()
    synth_pcm: list[int] = []
    for fi in range(n_frames):
        frame = pcm[fi * SAMPLES_PER_FRAME : (fi + 1) * SAMPLES_PER_FRAME]
        fields = analyze_frame(frame, enc)
        blobs.extend(pack_fields(fields))
        synth_pcm.extend(reference_synth_frame(fields, synth_state))
    return bytes(blobs), synth_pcm


def build_bitstream(n_bits: int) -> bytes:
    """Ones gate: one decode per speech consumer tick (matches 40-sample frames)."""
    n_bytes = (n_bits + 7) // 8
    return b"\xff" * n_bytes


def pack(audio: Path, out_stem: Path) -> None:
    verify_field_table()
    wav, tmp = resolve_wav(audio, out_stem)
    try:
        samples, rate = read_wav_mono_s16(wav)
        frames, _synth = pcm_to_frames(samples, rate)
        n_frames = len(frames) // FRAME
        for i in range(n_frames):
            fr = frames[i * FRAME : (i + 1) * FRAME]
            fields = unpack_fields(fr)
            if fields is None:
                raise RuntimeError(f"packed frame {i} failed decoder header check")
        n_bits = max(n_frames, 32)
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
            "encoding": "pcm_vocoder_v1",
            "gate": "ones_per_frame",
            "samples_per_frame": SAMPLES_PER_FRAME,
            "rate": DST_RATE,
            "payload_bits": PAYLOAD_BITS,
            "field_count": len(FIELD_TABLE),
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
