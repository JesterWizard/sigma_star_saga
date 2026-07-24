#include "dialogue_macros.h"

/* ROM 0x08075195 */
DIALOGUE_SCRIPT(0x08075195, scene_075195)
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "...",
      "She'll never trust me again. Not now."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "It was misplaced trust before.",
      "Now you can start earning the real thing."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "She would probably still be at Starbase 5.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "'Atta boy.",
      "Grab your bug hat and let's go."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Oh yeah. Guess I'll use it one last time.")
END_DIALOGUE_SCRIPT()
