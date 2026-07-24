#include "dialogue_macros.h"

/* ROM 0x08077047 */
DIALOGUE_SCRIPT(0x08077047, scene_077047)
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We've got to find the heart of the monster!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What's your plan?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We'll weaken it...",
      "Create an opening into its bloodstream!",
      "And insert the virus there!"
  )
END_DIALOGUE_SCRIPT()
