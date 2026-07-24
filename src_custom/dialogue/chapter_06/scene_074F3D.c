#include "dialogue_macros.h"

/* ROM 0x08074F3D */
DIALOGUE_SCRIPT(0x08074F3D, scene_074F3D)
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
      "We found Tierney's body. What remained of it anyway.",
      "Tell us how you did it."
  )
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
      "Need I remind you we have direct control over your body?",
      "Talk, you!!"
  )
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
      "Forget it. He won't talk.",
      "Shut him down so we can go."
  )
END_DIALOGUE_SCRIPT()
