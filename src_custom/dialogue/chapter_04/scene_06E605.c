#include "dialogue_macros.h"

/* ROM 0x0806E605 */
DIALOGUE_SCRIPT(0x0806E605, scene_06E605)
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "You put the moves on her?!")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "You'll have to sort things out later. Find a pond and submerge her.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But we're over a desert planet!")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Then take her somewhere with water! Just go, before she bursts right here on my floor!")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I can't go! I have to watch the virus!",
      "You're on your own you filthy scumbag!!"
  )
END_DIALOGUE_SCRIPT()
