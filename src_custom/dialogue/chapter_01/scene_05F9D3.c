#include "dialogue_macros.h"

/* ROM 0x0805F9D3 */
DIALOGUE_SCRIPT(0x0805F9D3, scene_05F9D3)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's with you Earthman?",
      "Never seen anyone get skinned before?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...er...")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "So you're the new guy, huh?")
END_DIALOGUE_SCRIPT()
