#include "dialogue_macros.h"

/* ROM 0x08065FBA */
DIALOGUE_SCRIPT(0x08065FBA, scene_065FBA)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "An access key?")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's for the Comm Room. I assigned it to a crewman just before you and Recker were transferred here.",
      "It looks like we've found our leak."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What's this all about anyway? Sounds like a witch hunt.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Family business. It doesn't concern you.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Fine. How bout getting the door on your way out.")
END_DIALOGUE_SCRIPT()
