#include "dialogue_macros.h"

/* ROM 0x0807403C */
DIALOGUE_SCRIPT(0x0807403C, scene_07403C)
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker. You shouldn't have given up the anti-virus!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "It had to be done.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "...")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well now Earth knows all six planets and has a clear shot at the bio weapon!",
      "Krill is doomed either way!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Not necessarily. I transferred the Sigma Fighter to our hangar. Load it up.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Where are you going?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Earth. To tie up some loose ends.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll be in the ship.")
END_DIALOGUE_SCRIPT()
