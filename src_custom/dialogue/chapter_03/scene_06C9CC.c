#include "dialogue_macros.h"

/* ROM 0x0806C9CC */
DIALOGUE_SCRIPT(0x0806C9CC, scene_06C9CC)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "When you come back we'll make things right! We can start over together!",
      "Promise me!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I...",
      "I will!",
      "But not for revenge...and not because of someone's orders!",
      "I won't let you go alone! I'll do it for you."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "We're out of time, GO!!")
END_DIALOGUE_SCRIPT()
