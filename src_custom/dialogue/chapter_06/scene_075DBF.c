#include "dialogue_macros.h"

/* ROM 0x08075DBF */
DIALOGUE_SCRIPT(0x08075DBF, scene_075DBF)
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "Welcome.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...",
      "Who's there?"
  )
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "You've managed to infiltrate my private chambers Mister Recker.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "So you're the all powerful Overlord?")
END_DIALOGUE_SCRIPT()
