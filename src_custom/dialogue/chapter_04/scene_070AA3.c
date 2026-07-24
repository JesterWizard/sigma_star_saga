#include "dialogue_macros.h"

/* ROM 0x08070AA3 */
DIALOGUE_SCRIPT(0x08070AA3, scene_070AA3)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme! This is it! Looks like it's linking to the first one.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Great. I bet all this running around is good for your glutes.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You should try it. Two planets in one day.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well this is a triathlon. You have one planet to go.",
      "Good luck."
  )
END_DIALOGUE_SCRIPT()
