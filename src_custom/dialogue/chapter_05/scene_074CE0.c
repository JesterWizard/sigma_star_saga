#include "dialogue_macros.h"

/* ROM 0x08074CE0 */
DIALOGUE_SCRIPT(0x08074CE0, scene_074CE0)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...Groan...")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Remember that little chip I put behind your ear?",
      "For all our little one-way chit chats?",
      "At this range it also gives me direct control of your central nervous system.",
      "It's time to say goodbye now."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "YAHRRGHHHH!!!")
END_DIALOGUE_SCRIPT()
