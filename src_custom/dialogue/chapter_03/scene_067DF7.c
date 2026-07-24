#include "dialogue_macros.h"

/* ROM 0x08067DF7 */
DIALOGUE_SCRIPT(0x08067DF7, scene_067DF7)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I want to know what this is all about, and you should too!",
      "This alien matter could be a weapon!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Who knows? It could just as easily be starship food!",
      "You know, your meddling is getting out of hand, and I'm going to be the one who gets bit on the backside!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'm going to look around inside the compound.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Go! I've had it up to here with you!")
END_DIALOGUE_SCRIPT()
