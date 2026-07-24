#include "dialogue_macros.h"

/* ROM 0x080749F9 */
DIALOGUE_SCRIPT(0x080749F9, scene_0749F9)
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You fool! This isn't about winning one silly campaign!",
      "It is about becoming the dominant force in the universe!",
      "I can accomplish this!",
      "When the Sleeping Flesh hatches, and each world is laid wasted, I alone will be able to master its awesome power! "
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You're mad!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I won't let anything stand between me and ultimate power!",
      "The virus! Destroy it!!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Stop!!")
END_DIALOGUE_SCRIPT()
