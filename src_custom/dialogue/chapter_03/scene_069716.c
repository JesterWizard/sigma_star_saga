#include "dialogue_macros.h"

/* ROM 0x08069716 */
DIALOGUE_SCRIPT(0x08069716, scene_069716)
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Say What?!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme, have you lost your mind?!")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Welllll! Pay attention Recker!",
      "Psyme here really knows how to suck up! She'll be a great officer in the new order, eh?"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I would serve you very well Commander.")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm sure.",
      "There's no shuttle between starbases for the moment. We're using every available ship to wipe out that nasty virus. Can't be too careful."
  )
END_DIALOGUE_SCRIPT()
