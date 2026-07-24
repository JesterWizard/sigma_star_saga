#include "dialogue_macros.h"

/* ROM 0x08067B6F */
DIALOGUE_SCRIPT(0x08067B6F, scene_067B6F)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll check.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I can't believe there are humans out here! You said this planet was remote even by Krill standards!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Uh oh.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Uh oh what?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Brace yourself. We've got another match.",
      "This makes three planets now, all shaking in unison.",
      "That means the humans knew about two of the planets before we did!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Come again?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Keep this under your hat. Earlier we met with the Tyrannical Overlord, head of the Krill High Command.",
      "He said that both the humans and the Krill were searching for six matching planets."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I know. I eavesdropped on your meeting.")
END_DIALOGUE_SCRIPT()
