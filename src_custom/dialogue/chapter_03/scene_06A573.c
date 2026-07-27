#include "event_macros.h"

/* Script id 268 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806A573. */
EVENT_SCRIPT_REPLACEMENT(0x0806A573, scene_06A573)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Blune! I found you!")
  END()

END_EVENT_SCRIPT()
