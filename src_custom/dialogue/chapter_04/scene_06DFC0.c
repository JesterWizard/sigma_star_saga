#include "event_macros.h"

/* Script id 320 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806DFC0. */
EVENT_SCRIPT_REPLACEMENT(0x0806DFC0, scene_06DFC0)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I got a FOSSIL! If I had ALL SEVEN, I could complete the skeleton!")
  END()

END_EVENT_SCRIPT()
