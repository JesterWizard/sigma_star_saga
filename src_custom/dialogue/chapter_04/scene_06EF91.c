#include "dialogue_macros.h"

/* ROM 0x0806EF91 */
DIALOGUE_SCRIPT(0x0806EF91, scene_06EF91)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme! There you are. How come you ran off like that?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Later. Something's not right here.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll check it.")
END_DIALOGUE_SCRIPT()
