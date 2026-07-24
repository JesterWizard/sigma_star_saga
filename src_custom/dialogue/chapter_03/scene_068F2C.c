#include "dialogue_macros.h"

/* ROM 0x08068F2C */
DIALOGUE_SCRIPT(0x08068F2C, scene_068F2C)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Did they see the flares?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes. The fleet is scrambled, so expect aerial skirmishes again...")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Good, now we can fly out of here!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "No, they detected us from the ground.",
      "We'll need to get to the hatch of Starbase 3 fast.",
      "It's landed somewhere up ahead."
  )
END_DIALOGUE_SCRIPT()
