#include "dialogue_macros.h"

/* ROM 0x080744DF */
DIALOGUE_SCRIPT(0x080744DF, scene_0744DF)
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm home. I never thought I'd see Earth again.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This is where we say our goodbyes. I have work to do.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "You're going after the virus?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'm going after Tierney.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "You know Psyme would kill me if I let something happen to you.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "She won't care.",
      "Not anymore."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's that supposed to mean?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "The Comm line was open. Psyme knows I'm her enemy now.",
      "I won't see her again.",
      "And I don't deserve to."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Please, look at me.",
      "Whatever happens, you mustn't let them destroy the original virus!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I won't.",
      "Wish me luck."
  )
END_DIALOGUE_SCRIPT()
