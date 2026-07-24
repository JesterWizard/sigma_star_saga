#include "dialogue_macros.h"

/* ROM 0x08072C6F */
DIALOGUE_SCRIPT(0x08072C6F, scene_072C6F)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Are you Folly? Or Sliss?")
  TALK(SPEAKER_FOLLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Folly senses STRENGTH. Can hear Lolly but no Sliss.",
      "Find Sliss for Folly!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You want me to find the third sister huh?")
  TALK(SPEAKER_FOLLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Go. Find.")
END_DIALOGUE_SCRIPT()
