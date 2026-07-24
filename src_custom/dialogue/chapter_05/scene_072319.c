#include "dialogue_macros.h"

/* ROM 0x08072319 */
DIALOGUE_SCRIPT(0x08072319, scene_072319)
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Officer on deck! What are your orders Commander?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Commander? Me?")
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "Absolutely! Just name it!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "How 'bout my own personal fighter?")
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "Any fighter in the fleet. ")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "No kidding! Hey, I know just the one!")
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "A collector, eh? I will have it delivered at once Sir.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Man, this is incredible!!")
END_DIALOGUE_SCRIPT()
