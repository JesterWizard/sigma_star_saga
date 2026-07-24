#include "dialogue_macros.h"

/* ROM 0x08073A0D */
DIALOGUE_SCRIPT(0x08073A0D, scene_073A0D)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What is this?")
  TALK(SPEAKER_IOT, SIDE_RIGHT, EXPR_NEUTRAL,
      "An essential mechanism used in life by the IOT.",
      "In times of need it may aid you.",
      "Nothing more can I do."
  )
END_DIALOGUE_SCRIPT()
