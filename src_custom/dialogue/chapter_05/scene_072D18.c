#include "dialogue_macros.h"

/* ROM 0x08072D18 */
DIALOGUE_SCRIPT(0x08072D18, scene_072D18)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You must be Sliss.")
  TALK(SPEAKER_SLISS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Sliss senses LIFE. Sisters can talk again. Mind clear now. ")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Fear, Strength, and Life. What does it mean?")
  TALK(SPEAKER_SLISS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Feelings of planet. Loud. Loudest at NEXUS. ")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "The planet is talking to you? Is the nexus the planet's voice?")
  TALK(SPEAKER_SLISS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Nexus. Go. Find.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But I've already been everywhere!")
  TALK(SPEAKER_SLISS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Nexus quiet before. Loud now.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Sigh...Guess I'll retrace my steps.")
END_DIALOGUE_SCRIPT()
