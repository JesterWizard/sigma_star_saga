#include "dialogue_macros.h"

/* ROM 0x080738E4 */
DIALOGUE_SCRIPT(0x080738E4, scene_0738E4)
  TALK(SPEAKER_IOT, SIDE_RIGHT, EXPR_NEUTRAL,
      "We will never forgive the Krill.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "He's not asking you to forget what happened!",
      "We just want to stop the Krill Overlord from controlling this weapon!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "If we don't do something, it will all happen again!",
      "Is that what you want?"
  )
  TALK(SPEAKER_IOT, SIDE_RIGHT, EXPR_NEUTRAL,
      "Perhaps...",
      "You deserve a chance."
  )
END_DIALOGUE_SCRIPT()
