#include "event_macros.h"

/* Script id 253 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08069ABC. */
EVENT_SCRIPT_REPLACEMENT(0x08069ABC, scene_069ABC)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Commander is one weird guy. I try to steer clear of him.")
  END()

END_EVENT_SCRIPT()
