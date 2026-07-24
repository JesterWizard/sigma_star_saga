#ifndef GUARD_DIALOGUE_MACROS_H
#define GUARD_DIALOGUE_MACROS_H

#include "constants/dialogue_speakers.h"

/* Authoring macros for talk scripts under data/dialogue/ (vanilla dump)
 * and src_custom/dialogue/ (editable; compiled into the ROM when
 * runtime.custom_dialogue is TRUE). Parsed by tools/compile_dialogue.py. */

#define SIDE_LEFT  '<'
#define SIDE_RIGHT '>'

#define EXPR_NEUTRAL 0
#define EXPR_ALT     1

#define DIALOGUE_SCRIPT(rom_addr, name)
#define END_DIALOGUE_SCRIPT()

/* TALK(speaker, side, expr, "page", "page", ...) */
#define TALK(...)

/* System / UI line with no portrait header. */
#define TEXT(...)

/* Yes/No prompt after a TEXT page (`\\x0c?` in the talk stream). */
#define CHOICE()

/* Chapter title card embedded in the talk stream. */
#define CHAPTER_TITLE(...)

/* Empty '#' stub that still consumes a script ID. */
#define EMPTY()

#endif /* GUARD_DIALOGUE_MACROS_H */
