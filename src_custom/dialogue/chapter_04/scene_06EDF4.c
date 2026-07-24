#include "dialogue_macros.h"

/* ROM 0x0806EDF4 */
DIALOGUE_SCRIPT(0x0806EDF4, scene_06EDF4)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I won't be growing them again anytime soon, so take care of those.",
      "Your parasite will do all the work I'm sure."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Wow. So...uh, what does this mean?",
      "Like, between us?"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It means I follow nature's course and eat you.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "CRIPES!!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm kidding. It just means I like you. That's it.",
      "There, you dragged it out of me. I know I'm Krill so just forget I said it already."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme-")
END_DIALOGUE_SCRIPT()
