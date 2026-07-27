#include "event_macros.h"

/* Script id 108 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806059D. */
EVENT_SCRIPT_REPLACEMENT(0x0806059D, scene_06059D)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Oh, man! Here it comes!")
  END()

END_EVENT_SCRIPT()
