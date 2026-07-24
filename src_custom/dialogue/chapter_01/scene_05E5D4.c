#include "dialogue_macros.h"

/* ROM 0x0805E5D4 */
DIALOGUE_SCRIPT(0x0805E5D4, scene_05E5D4)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Ugghhhh...",
      "Who...",
      "Who are you?"
  )
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "You will address me as Commander Bloss!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "My gun...",
      "You gave it back?"
  )
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "I've given you more than that, if you'd bothered to look!",
      "I've also fitted you with a Krill parasite!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What is this?! Some kind of body armor?",
      "Aaaah!"
  )
END_DIALOGUE_SCRIPT()
