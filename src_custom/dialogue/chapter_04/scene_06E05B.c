#include "event_macros.h"

/* Script id 323 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E05B. */
EVENT_SCRIPT_REPLACEMENT(0x0806E05B, scene_06E05B)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I got a FOSSIL! Only 3 more to go!")
  END()

END_EVENT_SCRIPT()
