#include "dialogue_macros.h"

/* ROM 0x0805FD1C */
DIALOGUE_SCRIPT(0x0805FD1C, scene_05FD1C)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "If everyone's here, let me go over our mission.",
      "This starbase has been conducting scans of the nearby Forest Planet for the past month.",
      "Krill High Command is no longer satisfied with long range scans.",
      "They think landing a team on the surface will yield different results."
  )
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "What kind of data are we supposed to be looking for?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "High Command knows, but they aren't saying. There is something about the planet that makes it unique. ",
      "Whatever we're looking for, it should turn up in routine geological surveying."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hah. Now there's something we have in common! Arrogant top brass!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Enough out of you Earthman.",
      "Why don't you make yourself useful at that NAV station.",
      "Drop us out of orbit and take us down. Don't worry, even a child can do it."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This \"star\" base can land?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Would you rather we call it a land base? Shut up and do as you're ordered!")
END_DIALOGUE_SCRIPT()
