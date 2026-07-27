#include "event_macros.h"

/* Script id 113 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08060F7A. */
EVENT_SCRIPT_REPLACEMENT(0x08060F7A, scene_060F7A)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Data Plug's for saving. Got it.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "When you're ready to hail a ship, just step into the middle of this circle.")
  END()

END_EVENT_SCRIPT()
