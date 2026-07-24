#include "dialogue_macros.h"

/* ROM 0x0806764C */
DIALOGUE_SCRIPT(0x0806764C, scene_06764C)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hmm...Huh?",
      "What's going on?"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Didn't you feel that? We've landed.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Oh, yeah. Sure is quiet.")
END_DIALOGUE_SCRIPT()
