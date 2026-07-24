#include "dialogue_macros.h"

/* ROM 0x0805CCBA */
DIALOGUE_SCRIPT(0x0805CCBA, scene_05CCBA)
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "We won?  This is huge!\nWe should celebrate!",
      "I'd call someone but the lines are all tied up!"
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "You'd better call your mom. I think I dropped one right over her neighborhood.")
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
      "Serious?")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Kidding. Relax.")
END_DIALOGUE_SCRIPT()
