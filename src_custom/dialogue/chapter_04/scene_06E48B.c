#include "dialogue_macros.h"

/* ROM 0x0806E48B */
DIALOGUE_SCRIPT(0x0806E48B, scene_06E48B)
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Holy Toledo! What have you done Recker?!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Nothing! Just a kiss!")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "You'll have to take her to a pool of water. Somewhere clean. The process can run its course there.",
      "I can't keep her on the station in her condition.",
      "Medic!"
  )
END_DIALOGUE_SCRIPT()
