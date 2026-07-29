#!/usr/bin/env python3
"""Host tests for pack_gax_speech (field table, encoder, reference synth)."""

from __future__ import annotations

import math
import struct
import sys
import tempfile
import wave
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "tools"))

from pack_gax_speech import (  # noqa: E402
    DST_RATE,
    FIELD_TABLE,
    FRAME,
    PAYLOAD_BITS,
    SAMPLES_PER_FRAME,
    analyze_frame,
    EncState,
    frame_nibble_ok,
    make_silence_frame,
    pack,
    pack_fields,
    pcm_compare,
    pcm_to_frames,
    reference_synth_frame,
    unpack_fields,
    unpack_header_fields,
    verify_field_table,
)


def write_sine_wav(path: Path, seconds: float = 0.25, freq: float = 220.0) -> None:
    n = int(DST_RATE * seconds)
    with wave.open(str(path), "wb") as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)
        wf.setframerate(DST_RATE)
        frames = bytearray()
        for i in range(n):
            s = int(16000 * math.sin(2 * math.pi * freq * i / DST_RATE))
            frames.extend(struct.pack("<h", s))
        wf.writeframes(frames)


def write_silence_wav(path: Path, seconds: float = 0.1) -> None:
    n = int(DST_RATE * seconds)
    with wave.open(str(path), "wb") as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)
        wf.setframerate(DST_RATE)
        wf.writeframes(b"\x00\x00" * n)


def write_voiced_wav(path: Path, seconds: float = 0.3, freq: float = 140.0) -> None:
    """Buzz + formant-ish tone (voiced speech stand-in)."""
    n = int(DST_RATE * seconds)
    with wave.open(str(path), "wb") as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)
        wf.setframerate(DST_RATE)
        frames = bytearray()
        for i in range(n):
            t = i / DST_RATE
            buzz = 1.0 if math.sin(2 * math.pi * freq * t) > 0 else -1.0
            form = 0.55 * math.sin(2 * math.pi * 700 * t) + 0.25 * math.sin(
                2 * math.pi * 1200 * t
            )
            s = int(9000 * (0.65 * buzz + 0.35 * form))
            frames.extend(struct.pack("<h", max(-32767, min(32767, s))))
        wf.writeframes(frames)


def check_blob(bin_path: Path, entry_path: Path, expect_silence: bool = False) -> None:
    payload = bin_path.read_bytes()
    import json

    meta = json.loads(entry_path.read_text())
    bit_offset = meta["bit_offset"]
    assert bit_offset % FRAME == 0, bit_offset
    frames = payload[:bit_offset]
    gates = payload[bit_offset:]
    assert len(frames) % FRAME == 0
    n = len(frames) // FRAME
    assert n >= 1
    assert meta["payload_bits"] == PAYLOAD_BITS
    assert meta["field_count"] == len(FIELD_TABLE)
    for i in range(n):
        fr = frames[i * FRAME : (i + 1) * FRAME]
        assert frame_nibble_ok(fr), f"frame {i} nibble"
        fields = unpack_fields(fr)
        assert fields is not None, f"frame {i} took -1 path"
        assert len(fields) == len(FIELD_TABLE)
        if expect_silence:
            assert all(v == 0 for v in fields.values()), f"silence frame {i} nonzero"
    assert (meta["size_flags"] & 0x80000000) != 0
    assert (meta["size_flags"] & 0x1FFFFFFF) == bit_offset
    assert gates and all(b == 0xFF for b in gates), "gate bits must be ones"


def test_field_table_roundtrip() -> None:
    verify_field_table()
    silence = make_silence_frame()
    assert len(silence) == FRAME
    assert unpack_fields(silence) == {o: 0 for o, _w, _n in FIELD_TABLE}
    assert unpack_header_fields(b"\xC0" + bytes(FRAME - 1)) is None

    probe = {o: (i * 5 + 1) & ((1 << w) - 1) for i, (o, w, _) in enumerate(FIELD_TABLE)}
    frame = pack_fields(probe)
    assert unpack_fields(frame) == probe
    print("ok: field_table_roundtrip")


def test_encoder_pcm() -> None:
    with tempfile.TemporaryDirectory() as td:
        td_path = Path(td)
        cases = (
            ("sine", write_sine_wav, False),
            ("silence", write_silence_wav, True),
            ("voiced", write_voiced_wav, False),
        )
        for name, writer, expect_silence in cases:
            wav = td_path / f"{name}.wav"
            stem = td_path / name
            writer(wav)
            pack(wav, stem)
            check_blob(
                stem.with_suffix(".bin"),
                stem.with_suffix(".entry.json"),
                expect_silence=expect_silence,
            )
            print(f"ok: pack_{name}")

        # Bounded PCM error vs host reference synth (encoder→synth loop).
        wav = td_path / "voiced2.wav"
        write_voiced_wav(wav, seconds=0.2, freq=160.0)
        with wave.open(str(wav), "rb") as wf:
            raw = wf.readframes(wf.getnframes())
        samples = list(struct.unpack("<" + "h" * (len(raw) // 2), raw))
        _frames, synth = pcm_to_frames(samples, DST_RATE)
        # Re-analyse original vs synth energy envelope — not sample-identical,
        # but RMS of synth should be in the same order of magnitude.
        src = samples[: len(synth)]
        err = pcm_compare(src, synth)
        src_rms = math.sqrt(sum(s * s for s in src) / max(1, len(src)))
        syn_rms = math.sqrt(sum(s * s for s in synth) / max(1, len(synth)))
        assert syn_rms > 100.0, syn_rms
        assert err < src_rms * 2.5, (err, src_rms)
        print(f"ok: pcm_compare err={err:.1f} src_rms={src_rms:.1f} syn_rms={syn_rms:.1f}")


def test_cross_frame_state() -> None:
    enc = EncState()
    quiet = [0] * SAMPLES_PER_FRAME
    loud = [int(12000 * math.sin(2 * math.pi * 180 * i / DST_RATE)) for i in range(SAMPLES_PER_FRAME)]
    f0 = analyze_frame(quiet, enc)
    assert all(v == 0 for v in f0.values())
    f1 = analyze_frame(loud, enc)
    assert f1[0x3FA] > 0
    # Second loud frame should keep energy continuous (not collapse).
    f2 = analyze_frame(loud, enc)
    assert abs(f2[0x3FA] - f1[0x3FA]) <= 8
    synth = EncState()
    pcm = reference_synth_frame(f2, synth)
    assert len(pcm) == SAMPLES_PER_FRAME
    print("ok: cross_frame_state")


def main() -> int:
    test_field_table_roundtrip()
    test_cross_frame_state()
    test_encoder_pcm()
    print("test_gax_speech_pack: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
