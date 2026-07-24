#include "dialogue_macros.h"

/* ROM 0x0807672D */
DIALOGUE_SCRIPT(0x0807672D, scene_07672D)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Now let's settle this once and for all!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I won't fight you!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You don't have to! You're going to die either way!!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Look around!",
      "Your world is ending in chaos! ",
      "You've got to fight the real enemy!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm powering up weapons! Leave or else!!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I refuse to lose you again!!",
      "Psyme?!",
      "PSYME!!"
  )
END_DIALOGUE_SCRIPT()
