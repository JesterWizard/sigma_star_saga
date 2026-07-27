# Editable dialogue / scene scripts

Edit `chapter_*/scene_*.c`. With `custom_dialogue = TRUE` in
[`configs/runtime.c`](../../configs/runtime.c), `make` packs `TALK`/`TEXT`/…
into appended ROM banks.

Scenes that also own a cutscene FSM (e.g. Ch.1 opener) use ygodm8-style
`EVENT_SCRIPT_REPLACEMENT` + event macros from [`event_macros.h`](../../include/event_macros.h);
`compile_dialogue.py` still only emits talk-bank bytes from `TALK`/…

```bash
make
```

Vanilla reference dump (do not edit for patches): [`data/dialogue/`](../../data/dialogue/).
Docs: [`documentation/dialogue/`](../../documentation/dialogue/),
[`documentation/events/`](../../documentation/events/).
