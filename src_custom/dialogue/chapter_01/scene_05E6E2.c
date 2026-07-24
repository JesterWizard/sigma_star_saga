#include "dialogue_macros.h"

/* ROM 0x0805E6E2 */
DIALOGUE_SCRIPT(0x0805E6E2, scene_05E6E2)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "It's alive! It's biting my head! It's crushing my neck!")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Every Krill soldier wears a parasite! You are no different.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hey...",
      "I can breathe again...",
      "I feel...stronger than before."
  )
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "The physical advantages are just the beginning! But I'll explain all that later.",
      "I'm sure my medical staff mentioned my little game."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yeah. I won't go easy on you.")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Me? Hoh no! I just observe...and take a few bets.",
      "You'll be fighting Hen'nk, the previous champ. ",
      "Now get ready...",
      "Begin!"
  )
END_DIALOGUE_SCRIPT()
