#include "dialogue_macros.h"

/* ROM 0x080676B3 */
DIALOGUE_SCRIPT(0x080676B3, scene_0676B3)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "That was the loader arm dropping us I think.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Whoa...",
      "It's getting cold!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Sure is. Let's ditch this crate.",
      "That is, unless you'd like to stay in here."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Aww, it wasn't so bad.")
END_DIALOGUE_SCRIPT()
