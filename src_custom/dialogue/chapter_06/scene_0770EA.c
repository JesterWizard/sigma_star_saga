#include "dialogue_macros.h"

/* ROM 0x080770EA */
DIALOGUE_SCRIPT(0x080770EA, scene_0770EA)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Is that...the Overlord?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "He's merged with it!",
      "That's the weakest spot!",
      "Destroy it!"
  )
END_DIALOGUE_SCRIPT()
