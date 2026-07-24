#include "dialogue_macros.h"

/* ROM 0x08069938 */
DIALOGUE_SCRIPT(0x08069938, scene_069938)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You heard him Recker! Enjoy your pet, sir.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You! You did this just to spite me!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Maybe next time you'll remember your place.",
      "Come on!"
  )
END_DIALOGUE_SCRIPT()
