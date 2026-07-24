#include "dialogue_macros.h"

/* ROM 0x08064F8C */
DIALOGUE_SCRIPT(0x08064F8C, scene_064F8C)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What is this thing? It looks like a hockey puck.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's a very old weapon.",
      "Kick it to cut a path through enemies or obstacles that get in your way."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll have to give it a shot.",
      "What can I say? Thanks!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "If you boys are finished, I'd like to get this new data back to base.",
      "You did manage to take readings down there, correct?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "The existence of that creature should be data enough.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "All right then, let's get back to base. Maybe you can use that Puck thing to find a better route.")
END_DIALOGUE_SCRIPT()
