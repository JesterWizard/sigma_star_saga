#include "event_macros.h"

/* Script id 236 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080691B2. */
EVENT_SCRIPT_REPLACEMENT(0x080691B2, scene_0691B2)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What was that?")
  END()

END_EVENT_SCRIPT()
