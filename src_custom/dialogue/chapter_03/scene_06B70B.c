#include "event_macros.h"

/* Script id 283 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806B70B. */
EVENT_SCRIPT_REPLACEMENT(0x0806B70B, scene_06B70B)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Here's your box.")
  END()

END_EVENT_SCRIPT()
