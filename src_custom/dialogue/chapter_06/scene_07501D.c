#include "dialogue_macros.h"

/* ROM 0x0807501D */
DIALOGUE_SCRIPT(0x0807501D, scene_07501D)
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Scarlet? You shouldn't be here.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We're getting out of here. There's still time.")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "To do what?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Psyme's still out there somewhere. And I'll bet she's in trouble.")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "That doesn't concern us anymore.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Come on, you can't hide it from me. You love her.")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "...")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I know when I'm beaten. I'll stay out of the way.",
      "I just want one thing...",
      "For my troubles...",
      "..."
  )
END_DIALOGUE_SCRIPT()
