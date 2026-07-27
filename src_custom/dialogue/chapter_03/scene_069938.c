#include "event_macros.h"

/* Script id 250 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08069938. */
EVENT_SCRIPT_REPLACEMENT(0x08069938, scene_069938)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You heard him Recker! Enjoy your pet, sir.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You! You did this just to spite me!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Maybe next time you'll remember your place.",
      "Come on!"
  )
  END()

END_EVENT_SCRIPT()
