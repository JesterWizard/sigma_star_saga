#include "dialogue_macros.h"

/* ROM 0x0806B9A5 */
DIALOGUE_SCRIPT(0x0806B9A5, scene_06B9A5)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's happening?")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "A hostile organism is inside the ship! It's the virus!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "The virus was never fully developed. It must be the byproduct strain.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "How did it get here? Could one of us have been a carrier?")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "It must have been carried by one of the Earthlings!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Don't jump to conclusions! I'll check it out! You both wait here.")
END_DIALOGUE_SCRIPT()
