#include "dialogue_macros.h"

/* ROM 0x080715FB */
DIALOGUE_SCRIPT(0x080715FB, scene_0715FB)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "They've found us. Quick, who did this to you?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "The Tyrannical...Overlord...",
      "Turned on me...",
      "I...knew too much..."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll take care of the crew. Can you find your way out?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes...but...",
      "You mustn't let...",
      "anyone escape...",
      "...Not a single one, Recker."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Count on it.")
END_DIALOGUE_SCRIPT()
