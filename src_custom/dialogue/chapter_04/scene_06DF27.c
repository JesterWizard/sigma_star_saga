#include "event_macros.h"

/* Script id 317 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806DF27. */
EVENT_SCRIPT_REPLACEMENT(0x0806DF27, scene_06DF27)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I got a FOSSIL! If I had ALL TEN, I could complete the skeleton!")
  END()

END_EVENT_SCRIPT()
