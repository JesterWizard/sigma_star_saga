#include "dialogue_macros.h"

/* ROM 0x08073A8D */
DIALOGUE_SCRIPT(0x08073A8D, scene_073A8D)
  TALK(SPEAKER_IOT, SIDE_RIGHT, EXPR_NEUTRAL,
      "You must go.",
      "The child stirs. Soon all will end."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You heard him, let's get out of here!")
END_DIALOGUE_SCRIPT()
