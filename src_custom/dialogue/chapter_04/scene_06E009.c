#include "event_macros.h"

/* Script id 321 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E009. */
EVENT_SCRIPT_REPLACEMENT(0x0806E009, scene_06E009)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I got a FOSSIL! Only 5 more to go!")
  END()

END_EVENT_SCRIPT()
