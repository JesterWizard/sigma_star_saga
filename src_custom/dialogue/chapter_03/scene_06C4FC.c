#include "event_macros.h"

/* Script id 291 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806C4FC. */
EVENT_SCRIPT_REPLACEMENT(0x0806C4FC, scene_06C4FC)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Earth fleet approaching! Looks like a full battalion!")
  END()

END_EVENT_SCRIPT()
