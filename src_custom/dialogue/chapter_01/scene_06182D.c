#include "event_macros.h"

/* Script id 124 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806182D. */
EVENT_SCRIPT_REPLACEMENT(0x0806182D, scene_06182D)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "This starbase is a dive. I'd do anything to get transferred to a better one. Or at least to lose Commander Bloss.")
  END()

END_EVENT_SCRIPT()
