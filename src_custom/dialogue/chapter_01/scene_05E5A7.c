#include "event_macros.h"

/* Script id 79 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805E5A7. */
EVENT_SCRIPT_REPLACEMENT(0x0805E5A7, scene_05E5A7)

  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "What's this?!")
  END()

END_EVENT_SCRIPT()
