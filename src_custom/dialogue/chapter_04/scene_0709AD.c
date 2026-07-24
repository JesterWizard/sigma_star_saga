#include "dialogue_macros.h"

/* ROM 0x080709AD */
DIALOGUE_SCRIPT(0x080709AD, scene_0709AD)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme, come in!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Go ahead.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I found the first probe. I hope changing the transmission frequency does the job.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "We won't know till you find the next probe. Get out of there and haul your tail to the next planet.")
END_DIALOGUE_SCRIPT()
