#include "event_macros.h"

/* Script id 318 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806DF6E. */
EVENT_SCRIPT_REPLACEMENT(0x0806DF6E, scene_06DF6E)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I got a FOSSIL! Only 8 more to go!")
  END()

END_EVENT_SCRIPT()
