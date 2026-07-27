#include "event_macros.h"

/* Script id 381 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080722EF. */
EVENT_SCRIPT_REPLACEMENT(0x080722EF, scene_0722EF)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Professor Zart wants to see you!")
  END()

END_EVENT_SCRIPT()
