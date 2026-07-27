#include "event_macros.h"

/* Script id 325 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E0AD. */
EVENT_SCRIPT_REPLACEMENT(0x0806E0AD, scene_06E0AD)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I got a FOSSIL! Only 1 more to go!")
  END()

END_EVENT_SCRIPT()
