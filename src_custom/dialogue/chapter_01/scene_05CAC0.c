#include "dialogue_macros.h"

/* ROM 0x0805CAC0 */
DIALOGUE_SCRIPT(0x0805CAC0, scene_05CAC0)
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Score one for Earth!")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Yeah, guess we did OK.")
END_DIALOGUE_SCRIPT()
