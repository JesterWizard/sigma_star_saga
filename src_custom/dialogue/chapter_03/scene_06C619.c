#include "dialogue_macros.h"

/* ROM 0x0806C619 */
DIALOGUE_SCRIPT(0x0806C619, scene_06C619)
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker! I need you! Earth is here for the black box. Get to the launch bay! We have a surprise waiting for them!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What kind of surprise?")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "We recovered a downed ship in last month's attack on your homeworld. It's fully restored, fitted with Krill weapons.",
      "This is your chance to shine, boy.",
      "As of this moment, you're a double agent! Fly into the fleet, get inside their lines, and wipe them out!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But...Scarlet...")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "No arguing! They are right on top of us!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Not until you let her go.")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Do this and the Earth girl is yours!",
      "GO!!"
  )
END_DIALOGUE_SCRIPT()
