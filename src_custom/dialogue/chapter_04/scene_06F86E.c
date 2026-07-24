#include "dialogue_macros.h"

/* ROM 0x0806F86E */
DIALOGUE_SCRIPT(0x0806F86E, scene_06F86E)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well well! Looks like I didn't get that memo.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme! I can explain-")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't bother. I heard enough.",
      "It's Mister Tierney I want to talk to."
  )
END_DIALOGUE_SCRIPT()
