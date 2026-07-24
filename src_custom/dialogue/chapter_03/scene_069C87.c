#include "dialogue_macros.h"

/* ROM 0x08069C87 */
DIALOGUE_SCRIPT(0x08069C87, scene_069C87)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's going on?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "There's no contact with Starbase 2.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It must be Zelly. I wouldn't put it past her to can the entire staff.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Maybe Blune too.")
  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "This is highly irregular. I'll notify Commander Nomak.",
      "There should be a shuttle in the bay by now. Take it back to Starbase 2 and check it out."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Will do.")
END_DIALOGUE_SCRIPT()
