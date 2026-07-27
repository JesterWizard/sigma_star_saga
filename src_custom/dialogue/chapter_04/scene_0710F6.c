#include "event_macros.h"

/* Script id 364 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080710F6. */
EVENT_SCRIPT_REPLACEMENT(0x080710F6, scene_0710F6)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "I'm new here. I don't know much of anybody.")
  END()

END_EVENT_SCRIPT()
