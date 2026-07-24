#include "dialogue_macros.h"

/* ROM 0x080603F8 */
DIALOGUE_SCRIPT(0x080603F8, scene_0603F8)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hot! I was hoping we'd see some action!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Where did they go?!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "The ships are spooked by something. They're summoning pilots.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You mean they can just suck people into the sky?!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Sure! That's what your parasite is for, idiot!",
      "It's a link between you and your ship!",
      "If you're summoned, just take aim and destroy anything that moves. Once the ship relaxes, you'll be sent back."
  )
END_DIALOGUE_SCRIPT()
