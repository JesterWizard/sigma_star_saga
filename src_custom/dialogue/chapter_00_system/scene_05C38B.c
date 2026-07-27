#include "event_macros.h"

/* Script id 23 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C38B. */
EVENT_SCRIPT_REPLACEMENT(0x0805C38B, scene_05C38B)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Sir! ")
  END()

END_EVENT_SCRIPT()
