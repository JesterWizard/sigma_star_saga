#include "dialogue_macros.h"

/* ROM 0x08072BCA */
DIALOGUE_SCRIPT(0x08072BCA, scene_072BCA)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "There you are! What are you doing?")
  TALK(SPEAKER_LOLLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Lolly senses FEAR. Lolly can not hear sisters.",
      "Get Folly. Get Sliss."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You want me to get your sisters?")
  TALK(SPEAKER_LOLLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Go. Find.")
END_DIALOGUE_SCRIPT()
