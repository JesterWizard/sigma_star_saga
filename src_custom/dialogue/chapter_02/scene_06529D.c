#include "event_macros.h"

/* Script id 172 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806529D. */
EVENT_SCRIPT_REPLACEMENT(0x0806529D, scene_06529D)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "I'm reading a power drain from the Comm Room. Has someone been making long distance calls again?")
  END()

END_EVENT_SCRIPT()
