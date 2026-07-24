#include "dialogue_macros.h"

/* ROM 0x08069B9F */
DIALOGUE_SCRIPT(0x08069B9F, scene_069B9F)
  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hey Recker. Commander had me look into that transfer for your pal Blune.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "And?")
  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "You said he's on Starbase 2 working for Commander Zelly? Because it looks like that starbase has gone off line.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Is that normal?")
  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "No.")
END_DIALOGUE_SCRIPT()
