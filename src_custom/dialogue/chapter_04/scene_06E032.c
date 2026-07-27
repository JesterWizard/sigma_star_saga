#include "event_macros.h"

/* Script id 322 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E032. */
EVENT_SCRIPT_REPLACEMENT(0x0806E032, scene_06E032)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I got a FOSSIL! Only 4 more to go!")
  END()

END_EVENT_SCRIPT()
