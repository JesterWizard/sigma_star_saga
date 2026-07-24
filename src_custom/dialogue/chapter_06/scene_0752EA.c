#include "dialogue_macros.h"

/* ROM 0x080752EA */
DIALOGUE_SCRIPT(0x080752EA, scene_0752EA)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This place is quiet as a crypt.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "According to this data, the tremors on the Forgotten Planet have backed off a bit. It should be safe to explore.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme wouldn't be there. She's probably gone to warn the Krill Overlord not to wake the Sleeping Flesh.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Sure enough. A shuttle left for Starbase 6 in orbit over the Krill Homeworld. We can follow her trail easily.",
      "I'll send the data to the Nav Room."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Once we reach Starbase 6 the Krill will be all over us.",
      "I want you to stay in the shuttle, understood?"
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "No problem. I'm not stepping foot in that place.")
END_DIALOGUE_SCRIPT()
