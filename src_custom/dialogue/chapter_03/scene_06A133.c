#include "event_macros.h"

/* Script id 262 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806A133. */
EVENT_SCRIPT_REPLACEMENT(0x0806A133, scene_06A133)

  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I want you to find him and finish him.")
  END()

END_EVENT_SCRIPT()
