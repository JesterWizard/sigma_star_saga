#include "dialogue_macros.h"

/* ROM 0x08063B83 */
DIALOGUE_SCRIPT(0x08063B83, scene_063B83)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Let's split up. Give a holler if you find a good beacon spot.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What am I looking for?")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "A discolored plot of soil is best. It means the ground is soft, but not flowing with magma. We'll need 3 BEACONS set up to triangulate properly.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Three beacons, got it.")
END_DIALOGUE_SCRIPT()
