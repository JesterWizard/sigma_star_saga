#include "dialogue_macros.h"

/* ROM 0x080677C0 */
DIALOGUE_SCRIPT(0x080677C0, scene_0677C0)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Where are we?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What happened to Starbase 3?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Looks like we've been delivered to the Ice Planet by mistake.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's no mistake. This is Zelly's idea of a joke.",
      "Or worse, she's ready to take things to the next level.",
      "I sort of went over her head earlier."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Oh, great. Thanks for dragging me down with ya.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What are friends for?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Friends, huh?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "We'd better look for signs of life, or a way to alert Starbase 3 that we're down here.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Call for help on that bikini slug.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I told you before! I can only communicate when the fleet is overhead.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Then I guess it's up to me to scout ahead.")
END_DIALOGUE_SCRIPT()
