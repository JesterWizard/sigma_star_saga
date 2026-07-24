#include "dialogue_macros.h"

/* ROM 0x0805E417 */
DIALOGUE_SCRIPT(0x0805E417, scene_05E417)
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Wake up kid, you're on!")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Moan...")
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "They're ready to skin you.")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Skin me? What do you mean?")
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't worry. It's not as bad as it sounds.",
      "They just want to fit you for a parasite."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Everything you said sounds bad.")
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Down the hall. You'll see.",
      "And if I might make a suggestion...",
      "Don't try to escape.",
      "It would be bad for both of us."
  )
END_DIALOGUE_SCRIPT()
