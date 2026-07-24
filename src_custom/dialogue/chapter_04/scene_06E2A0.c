#include "dialogue_macros.h"

/* ROM 0x0806E2A0 */
DIALOGUE_SCRIPT(0x0806E2A0, scene_06E2A0)
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Did you find the Genome?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Right here.")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Let's get started. We'll breed the virus in one tank, and the anti-virus in the other.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't worry Psyme, I didn't put the moves on loverboy.",
      "Err Psyme, are you all right?"
  )
END_DIALOGUE_SCRIPT()
