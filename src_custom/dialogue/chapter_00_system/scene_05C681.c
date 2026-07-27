#include "event_macros.h"

/* Script id 37 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C681. */
EVENT_SCRIPT_REPLACEMENT(0x0805C681, scene_05C681)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "I will live and die for the Krill cause!")
  END()

END_EVENT_SCRIPT()
