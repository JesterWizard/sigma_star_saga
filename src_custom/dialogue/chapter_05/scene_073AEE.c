#include "dialogue_macros.h"

/* ROM 0x08073AEE */
DIALOGUE_SCRIPT(0x08073AEE, scene_073AEE)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Where are your sisters going?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Who cares?! Let's get out of here!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "It's only a matter of time before these tremors reach lethal proportions!",
      "Hurry!"
  )
END_DIALOGUE_SCRIPT()
