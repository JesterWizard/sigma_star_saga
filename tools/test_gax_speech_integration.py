#!/usr/bin/env python3
"""Static integration checks for GAX vocoder playback wiring."""

from __future__ import annotations

import json
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "tools"))

from pack_gax_speech import FRAME, unpack_fields  # noqa: E402


def main() -> int:
    rom = (REPO / "sigma_star_saga.gba").read_bytes()
    # Consumer veneer present when custom_gax_audio built.
    off = 0x56A30
    assert rom[off : off + 2] == b"\x00\x4b", rom[off : off + 8].hex()
    hook = struct.unpack_from("<I", rom, off + 4)[0]
    assert hook & 1, hex(hook)

    entry = json.loads(
        (REPO / "sound/voice/built/01_tierney_briefing.entry.json").read_text()
    )
    assert entry["encoding"] == "pcm_vocoder_v1"
    blob = (REPO / "sound/voice/built/01_tierney_briefing.bin").read_bytes()
    frames = blob[: entry["bit_offset"]]
    assert len(frames) % FRAME == 0
    nonzero = 0
    for i in range(0, len(frames), FRAME):
        fields = unpack_fields(frames[i : i + FRAME])
        assert fields is not None
        if any(fields.values()):
            nonzero += 1
    assert nonzero > 0, "Tierney blob must not be silence-only"

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

    gax = (REPO / "src/gax.c").read_text()
    assert "flags & ~GAX_FLAG_SPEECH" in gax
    assert "GaxAttachSpeech()" in gax

    stop = hooks[hooks.find("GaxStopVoice") :]
    assert "GAX_SPEECH_INDEX_OFF" in stop
    assert "StopAllFx" not in stop.split("}")[0]

    print("test_gax_speech_integration: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
