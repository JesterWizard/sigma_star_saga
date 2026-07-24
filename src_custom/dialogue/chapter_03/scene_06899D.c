#include "dialogue_macros.h"

/* ROM 0x0806899D */
DIALOGUE_SCRIPT(0x0806899D, scene_06899D)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You're not thinking about trading up are you?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Of course not!",
      "I mean, it's not like...",
      "Bah, let's just go! Come on!"
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Where are we going?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "There's a Krill starbase in orbit. We need to let them know to scramble the fleet so we can warp out of here.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What about her? She can't be summoned into a Krill ship.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I don't want to go on a Krill ship!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Well we can't stay here.",
      "Hmmmm."
  )
END_DIALOGUE_SCRIPT()
