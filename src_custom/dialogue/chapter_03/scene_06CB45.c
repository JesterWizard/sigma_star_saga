#include "dialogue_macros.h"

/* ROM 0x0806CB45 */
DIALOGUE_SCRIPT(0x0806CB45, scene_06CB45)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I can't stay! I have to get out of here.")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Calm down, Recker. It'll pass...")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I just destroyed the last line of defense for my people!")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "You are safe here.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Ek is dead.",
      "I found his body in the launch bay."
  )
END_DIALOGUE_SCRIPT()
