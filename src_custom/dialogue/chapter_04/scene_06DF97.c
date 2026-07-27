#include "event_macros.h"

/* Script id 319 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806DF97. */
EVENT_SCRIPT_REPLACEMENT(0x0806DF97, scene_06DF97)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I got a FOSSIL! Only 7 more to go!")
  END()

END_EVENT_SCRIPT()
