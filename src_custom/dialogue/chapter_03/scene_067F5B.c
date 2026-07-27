#include "event_macros.h"

/* Script id 206 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08067F5B. */
EVENT_SCRIPT_REPLACEMENT(0x08067F5B, scene_067F5B)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What is this?!")
  END()

END_EVENT_SCRIPT()
