#include "dialogue_macros.h"

/* ROM 0x08071EFC */
DIALOGUE_SCRIPT(0x08071EFC, scene_071EFC)
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "That was a close one.",
      "Thank goodness you got my note about hiding in the ruins."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Note?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I left you a note on the table!",
      "The note written in English?!",
      "The note only you could read?!",
      "Wow. You are a total brain case."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Ah. Errr, what matters is that you have the anti-virus, right?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes. Take it.")
END_DIALOGUE_SCRIPT()
