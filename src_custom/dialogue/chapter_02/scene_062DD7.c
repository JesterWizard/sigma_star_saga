#include "dialogue_macros.h"

/* ROM 0x08062DD7 */
DIALOGUE_SCRIPT(0x08062DD7, scene_062DD7)
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "He will be commanding the mission.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't expect any special treatment Recker. I'd just as soon see you dead and out of my hair.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Thanks.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "But since I have no say in the matter, welcome aboard.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Quite a sense of humor you have.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Not really.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "That will do. Blune, please bring Psyme in for her briefing.",
      "Recker, you are free to tour the starbase. You will find your quarters at the end of the hall.",
      "Dismissed."
  )
END_DIALOGUE_SCRIPT()
