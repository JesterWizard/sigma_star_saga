#include "dialogue_macros.h"

/* ROM 0x08076F1F */
DIALOGUE_SCRIPT(0x08076F1F, scene_076F1F)
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "What is going on! What was that thing!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This is the end!! Just hold onto me!!")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "No...We have to do something!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Like what?!")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I still have the virus...",
      "We can use it, but we must move quickly!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But Tierney destroyed it! I saw him!")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Trust me!!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "ZELLY! GET US OUT OF HERE!!")
END_DIALOGUE_SCRIPT()
