#include "dialogue_macros.h"

/* ROM 0x0806CF22 */
DIALOGUE_SCRIPT(0x0806CF22, scene_06CF22)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "So the virus was never intended to destroy the Krill?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "No. But it destroyed the tissue sample we were given. We just don't know what the sample was!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You'd better come with us.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Thanks Psyme. And thanks for saving me from this maniac.")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "The Krill Overlord will not allow this!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What about him?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Leave him! It's the only way to guarantee a Starbase will be waiting for us.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "We'd better get to the launch bay fast.")
END_DIALOGUE_SCRIPT()
