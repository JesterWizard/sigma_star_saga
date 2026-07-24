#include "dialogue_macros.h"

/* ROM 0x08071B07 */
DIALOGUE_SCRIPT(0x08071B07, scene_071B07)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This is Recker!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I found our stray sheep.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Where is she?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll tell you in person. Get to Starbase 4 on the double.")
END_DIALOGUE_SCRIPT()
