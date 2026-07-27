# sound/

Custom GAX audio sources. See [`documentation/gax-audio.md`](../documentation/gax-audio.md).

| Path | Role |
|------|------|
| `music/*.json` | BGM catalog entries (`id`, optional `vanilla_module`, or instruments/patterns) |
| `music/samples/` | WAV samples for packed songs |
| `voice/*.json` | Voice catalog (`id`, `src` / `wav` / `mp3`) |
| `voice/*.{wav,mp3}` | Source audio; MP3 needs **ffmpeg** at build time |

Wire clips in dialogue with `TALK(..., VOICE(id), "…")` / `VOICE_STOP` (see gax-audio.md).

`make` runs `tools/build_gax_catalog.py` → `src_custom/generated/gax_catalog.c` + `include/gax_catalog.h`.
