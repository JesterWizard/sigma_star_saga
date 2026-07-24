#include "dialogue_macros.h"

/* ROM 0x08071BC9 */
DIALOGUE_SCRIPT(0x08071BC9, scene_071BC9)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You've located Scarlet?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "The reason my surface scans couldn't pick her up before is because she's underground.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Where underground?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Looks like a temple of some sort. Beneath the ruins.",
      "She must have the anti-virus. Why else would she hide there?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "When Blune arrived to steal the virus, she must have run off with it.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Which means Blune killed Professor Zart to get it.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Makes sense.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I've moved a Krill Fighter to the launch bay and loaded the coordinates into the NAV computer.",
      "Pilot the ship into the temple, make your way to the central chamber and grab Scarlet.",
      "Oh, and make sure you SAVE your progress before launching!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll be back in no time.")
END_DIALOGUE_SCRIPT()
