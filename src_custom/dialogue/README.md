# Editable dialogue (in-game)

Edit the `chapter_*/scene_*.c` files here. With `custom_dialogue = TRUE` in
[`configs/runtime.c`](../../configs/runtime.c), `make` compiles them into
appended ROM banks and redirects the vanilla bank pointer table so your text
appears in-game.

```bash
make
```

Vanilla reference dump (do not edit for patches): [`data/dialogue/`](../../data/dialogue/).
Docs: [`documentation/dialogue/`](../../documentation/dialogue/).
