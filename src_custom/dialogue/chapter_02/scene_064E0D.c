#include "dialogue_macros.h"

/* ROM 0x08064E0D */
DIALOGUE_SCRIPT(0x08064E0D, scene_064E0D)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You're back! What happened?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "We ran into trouble. Some sort of giant worm was holed up inside the fissure.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "It was Recker's fancy flying that saved our skins. I was sure we were dead meat.",
      "When it was time to fire I just froze up."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Forget it.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "I can't. If not for you, I'd be worm food now.",
      "Ah, I've got just the thing to thank you...Here."
  )
END_DIALOGUE_SCRIPT()
