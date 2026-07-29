#!/usr/bin/env python3
"""Host round-trip + catalog checks for GAX voice DPCM."""

from __future__ import annotations

import json
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "tools"))

from pack_gax_voice_fx import (  # noqa: E402
    GAX_VOICE_DECODE_BYTES,
    decode_dpcm4_u8,
    encode_dpcm4_u8,
    trim_near_silence,
)


def test_round_trip() -> None:
    # Small slopes fit step=1 exactly; large slopes need auto step.
    gentle = bytes([0x80 + (i % 5) - 2 for i in range(64)])
    blob = encode_dpcm4_u8(gentle, step=1)
    assert blob[:4] == b"VD1\0"
    assert blob[9] == 1
    assert decode_dpcm4_u8(blob) == gentle

    # Speech-like jumps: auto step should keep MAE low vs step=1.
    pcm = bytes([0x80] * 4 + list(range(0x40, 0xC0)) + [0x80] * 4)
    trimmed, _, _ = trim_near_silence(pcm)
    blob = encode_dpcm4_u8(trimmed)
    step = blob[9]
    assert step >= 1
    out = decode_dpcm4_u8(blob)
    assert len(out) == len(trimmed)
    mae = sum(abs(a - b) for a, b in zip(trimmed, out)) / len(trimmed)
    assert mae < 4.0, mae
    # Auto step beats naive step=1 on this ramp.
    naive = encode_dpcm4_u8(trimmed, step=1)
    naive_out = decode_dpcm4_u8(naive)
    naive_mae = sum(abs(a - b) for a, b in zip(trimmed, naive_out)) / len(trimmed)
    assert mae <= naive_mae


def test_clamp_limit() -> None:
    try:
        encode_dpcm4_u8(bytes([0x80]) * (GAX_VOICE_DECODE_BYTES + 1))
        raise AssertionError("expected overflow reject")
    except ValueError as e:
        assert "GAX_VOICE_DECODE_BYTES" in str(e)


def test_catalog_and_built() -> None:
    dpcm = REPO / "sound/voice/built/01_tierney_briefing.fx.dpcm"
    entry = json.loads(dpcm.with_suffix(dpcm.suffix + ".entry.json").read_text())
    assert entry["encoding"] == "dpcm4_u8"
    assert entry.get("dpcm_step", 1) >= 1
    blob = dpcm.read_bytes()
    assert len(blob) == entry["bytes"]
    assert entry["samples"] > 0
    assert entry["bytes"] < entry["pcm_bytes"]  # compressed vs raw PCM
    assert blob[9] == entry["dpcm_step"] or (blob[9] == 0 and entry["dpcm_step"] == 1)
    out = decode_dpcm4_u8(blob)
    assert len(out) == entry["samples"]

    catalog = (REPO / "src_custom/generated/gax_catalog.c").read_text()
    assert "sVoiceFxDpcm_1" in catalog
    assert "sVoiceFxPcm_" not in catalog
    assert "gGaxVoiceDpcmTable" in catalog
    assert "gGaxFxWaveSetEx" in catalog

    voices = (REPO / "sound/voice/VOICES.md").read_text()
    assert "dpcm4_u8" in voices or "DPCM" in voices or "4-bit delta DPCM" in voices

    hooks = (REPO / "src_custom/gax_audio_hooks.c").read_text()
    assert "GaxVoiceDecodeDpcm" in hooks
    assert "gVoiceDecodeBuf" in hooks
    assert "gGaxFxWaveSetRam" in hooks


def main() -> int:
    test_round_trip()
    test_clamp_limit()
    # Built artifacts exist after catalog rebuild; skip soft if missing.
    dpcm = REPO / "sound/voice/built/01_tierney_briefing.fx.dpcm"
    if dpcm.is_file() and (REPO / "src_custom/generated/gax_catalog.c").is_file():
        test_catalog_and_built()
    else:
        print("test_gax_voice_dpcm: round-trip OK (catalog not built yet)")
        return 0
    print("test_gax_voice_dpcm: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
