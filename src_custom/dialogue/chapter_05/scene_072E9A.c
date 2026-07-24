#include "dialogue_macros.h"

/* ROM 0x08072E9A */
DIALOGUE_SCRIPT(0x08072E9A, scene_072E9A)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "There you are! Is this place the Nexus you were talking about?")
  TALK(SPEAKER_SISTERS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes. Very loud!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I don't hear anything. ")
  TALK(SPEAKER_SISTERS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Loud! Too loud!! Hurting!")
END_DIALOGUE_SCRIPT()
