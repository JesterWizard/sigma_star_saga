#include "event_macros.h"

/* Script id 177 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080656F9. */
EVENT_SCRIPT_REPLACEMENT(0x080656F9, scene_0656F9)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This must be right above the lab...")
  END()

END_EVENT_SCRIPT()
