#include "event_macros.h"

/* Script id 145 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806301A. */
EVENT_SCRIPT_REPLACEMENT(0x0806301A, scene_06301A)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Looks like the Fire Planet is pretty unstable. How will we land there?")
  END()

END_EVENT_SCRIPT()
