#include "dialogue_macros.h"

/* ROM 0x0805CAF6 */
DIALOGUE_SCRIPT(0x0805CAF6, scene_05CAF6)
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Way to go Reck! Once we salvage that hunk of junk you destroyed we'll be able to make better ships and weapons!")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "You want to thank me?  Name your next fighter after Sigma Team!")
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
      "Or after you. The Recker 3000!",
      "Ha Ha Ha!"
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "I never liked you much.")
END_DIALOGUE_SCRIPT()
