#!/usr/bin/env python3
"""Static integration checks for GAX FX voice playback wiring."""

from __future__ import annotations

import json
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]


def main() -> int:
    rom = (REPO / "sigma_star_saga.gba").read_bytes()
    # Consumer veneer present when custom_gax_audio built.
    off = 0x56A30
    assert rom[off : off + 2] == b"\x00\x4b", rom[off : off + 8].hex()
    hook = struct.unpack_from("<I", rom, off + 4)[0]
    assert hook & 1, hex(hook)

    fx = REPO / "sound/voice/built/01_tierney_briefing.fx.dpcm"
    entry = json.loads(fx.with_suffix(fx.suffix + ".entry.json").read_text())
    assert entry["encoding"] == "dpcm4_u8"
    dpcm = fx.read_bytes()
    assert len(dpcm) == entry["bytes"] > 0
    assert entry["samples"] > 0
    assert entry["bytes"] < entry.get("pcm_bytes", entry["samples"] + 1)
    assert entry.get("trim_lead", 0) + entry.get("trim_trail", 0) >= 0
    # Near-silence ends should have been trimmed when present in the source.
    assert entry["samples"] <= entry.get("raw_samples", entry["samples"])

    catalog = (REPO / "src_custom/generated/gax_catalog.c").read_text()
    assert "sVoiceSlotPresent" in catalog
    assert "sVoiceBlob_" not in catalog
    assert "sVoiceFxDpcm_1" in catalog
    assert "sVoiceFxPcm_" not in catalog
    assert "gGaxVoiceDpcmTable" in catalog

    clips = json.loads((REPO / "sound/voice/voice_clips.json").read_text())
    assert "chapter_01" in clips
    assert any(
        c.get("id") == "GAX_VOICE_TIERNEY_BRIEFING" for c in clips["chapter_01"]
    )
    assert all("chapter" in c for c in clips["chapter_01"])

    scene = (REPO / "src_custom/dialogue/chapter_01/scene_05D06C.c").read_text()
    assert "VOICE(GAX_VOICE_TIERNEY_BRIEFING)" in scene
    assert "VOICE_STOP" in scene

    hooks = (REPO / "src_custom/gax_audio_hooks.c").read_text()
    assert "GAX_WS_RATE_OFF" in hooks
    assert "GAX_FLAG_SPEECH" in hooks
    assert "GaxSpeechConsumer__Replacement" in hooks
    assert "GaxVoiceDecodeDpcm" in hooks
    assert "gVoiceDecodeBuf" in hooks

    gax = (REPO / "src/gax.c").read_text()
    assert "flags & ~GAX_FLAG_SPEECH" in gax
    assert "GaxAttachSpeech()" in gax

    stop = hooks[hooks.find("GaxStopVoice") :]
    assert "StopFxChannel" in stop.split("APPEND_TEXT")[0]
    assert "StopAllFx" not in stop.split("}")[0]

    print("test_gax_speech_integration: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
