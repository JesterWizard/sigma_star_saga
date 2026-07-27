#include "event_macros.h"

/* Script id 199 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08067A25. */
EVENT_SCRIPT_REPLACEMENT(0x08067A25, scene_067A25)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Holler if you find anything.")
  END()

END_EVENT_SCRIPT()
