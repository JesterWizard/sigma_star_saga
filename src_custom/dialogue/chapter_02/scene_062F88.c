#include "dialogue_macros.h"

/* ROM 0x08062F88 */
DIALOGUE_SCRIPT(0x08062F88, scene_062F88)
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker...come in. Find the COMM ROOM and contact me ASAP.",
      "Tierney out!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Great. I have no access to that room. Now what am I going to do?")
END_DIALOGUE_SCRIPT()
