#include "event_macros.h"

/* Script id 69 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805D867. */
EVENT_SCRIPT_REPLACEMENT(0x0805D867, scene_05D867)

  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "You mentioned a plan to get me in.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's already begun.")
  END()

END_EVENT_SCRIPT()
