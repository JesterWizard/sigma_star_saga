#include "dialogue_macros.h"

/* ROM 0x0805FA8A */
DIALOGUE_SCRIPT(0x0805FA8A, scene_05FA8A)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Thanks, I guess.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "But then, my parasite is a primitive animal.",
      "How about you?  What did they fit you with?"
  )
END_DIALOGUE_SCRIPT()
