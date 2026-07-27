#include "event_macros.h"

/* Script id 341 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E9D8. */
EVENT_SCRIPT_REPLACEMENT(0x0806E9D8, scene_06E9D8)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Here goes.")
  END()

END_EVENT_SCRIPT()
