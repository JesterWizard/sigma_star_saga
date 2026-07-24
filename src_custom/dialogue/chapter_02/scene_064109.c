#include "dialogue_macros.h"

/* ROM 0x08064109 */
DIALOGUE_SCRIPT(0x08064109, scene_064109)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "That does it. All three beacons should be cleared.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "We have a strong signal from all three. Nice flying Recker.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Base reports all force fields are now active. We should be able to explore those new areas now.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Remember, our mission objective is to find a fissure deep enough to take a clear reading of the planet.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll head back in the direction we came. There were some places I want to check out.")
END_DIALOGUE_SCRIPT()
