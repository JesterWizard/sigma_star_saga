#include "dialogue_macros.h"

/* ROM 0x0805C5AD */
DIALOGUE_SCRIPT(0x0805C5AD, scene_05C5AD)
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "What time is it?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Sorry, I don't have a watch.")
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's a watch?")
END_DIALOGUE_SCRIPT()
