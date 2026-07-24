#include "dialogue_macros.h"

/* ROM 0x0807444B */
DIALOGUE_SCRIPT(0x0807444B, scene_07444B)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Blune? I'm so sorry.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You're too late. The anti-virus is ours.",
      "Turn yourself in now and I'll guarantee your freedom!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You win. ")
END_DIALOGUE_SCRIPT()
