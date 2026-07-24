#include "dialogue_macros.h"

/* ROM 0x0806244B */
DIALOGUE_SCRIPT(0x0806244B, scene_06244B)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You ready to leave this heap?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "More than.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ok. Go to the NAV COM just like you did before.",
      "But this time, instead of landing, choose FIRE PLANET to enter that destination.",
      "The ELEVATOR in the next room will take you straight to the launch bay.",
      "The list of available starbases will change from time to time, so just know this might be a one way trip."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What about the Forest Planet? ",
      "Shouldn't I toughen up my parasite before we leave?"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's up to you.",
      "But the sooner we get to Starbase 2, the better I'll like it.",
      "I'll be in the shuttle.",
      "Don't keep a lady waiting."
  )
END_DIALOGUE_SCRIPT()
