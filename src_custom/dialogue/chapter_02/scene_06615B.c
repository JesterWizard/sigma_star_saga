#include "event_macros.h"

/* Script id 187 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806615B. */
EVENT_SCRIPT_REPLACEMENT(0x0806615B, scene_06615B)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "How could she sell us out like this!")
  END()

END_EVENT_SCRIPT()
