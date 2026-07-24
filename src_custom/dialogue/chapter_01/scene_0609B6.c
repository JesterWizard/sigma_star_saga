#include "dialogue_macros.h"

/* ROM 0x080609B6 */
DIALOGUE_SCRIPT(0x080609B6, scene_0609B6)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What have you men found?")
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "There is definitely something here, ma'am.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I feel it. Rumbling. Like a vehicle approaching.")
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's deep underground.",
      "I can't tell how far."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "This might be what we're looking for.",
      "Stay here and record it. Then return to the starbase with your readings."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Where are we going?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "To find out what's causing this hunk of rock to shake. Somehow I doubt High Command sent us here for a simple foot massage.")
END_DIALOGUE_SCRIPT()
