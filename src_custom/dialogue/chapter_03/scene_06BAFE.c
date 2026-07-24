#include "dialogue_macros.h"

/* ROM 0x0806BAFE */
DIALOGUE_SCRIPT(0x0806BAFE, scene_06BAFE)
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker! Report ASAP!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Roger that! Oh, I forgot it's a one way signal.",
      "I'll have to risk the COMM ROOM again."
  )
END_DIALOGUE_SCRIPT()
