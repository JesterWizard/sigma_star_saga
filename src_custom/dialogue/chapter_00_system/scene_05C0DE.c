#include "event_macros.h"

/* Script id 13 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C0DE. */
EVENT_SCRIPT_REPLACEMENT(0x0805C0DE, scene_05C0DE)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "When you visit the mess hall, don't get the special. Trust me on that one.")
  END()

END_EVENT_SCRIPT()
