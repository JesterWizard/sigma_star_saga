# Event Scripts

Cutscene-bearing `EVENT_SCRIPT` sources live here when they outgrow
`src_custom/dialogue/`. Today the Ch.1 cutscene scenes still sit beside their
talk banks (so `compile_dialogue.py` keeps packing `TALK` bytes).

| Scene | Talk id | FSM peel | Dialogue path |
|-------|---------|----------|---------------|
| `ch1_opener.c` | 51 | `CutsceneCh1Opener` @ `0x080523EC` | `src_custom/dialogue/chapter_01/scene_05C9E1.c` |
| `ch1_target_brief.c` | 109 | `CutsceneCh1TargetBrief` @ `0x0802BFA0` | `src_custom/dialogue/chapter_01/scene_0605BB.c` |

Thin wrappers below `#include` the dialogue scene so both trees stay in sync.

Build:

```bash
python3 tools/compile_events.py
python3 tools/compile_dialogue.py --src src_custom/dialogue --out /tmp/banks.c
```
