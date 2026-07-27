#include "event_macros.h"

/* Script id 32 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C594. */
EVENT_SCRIPT_REPLACEMENT(0x0805C594, scene_05C594)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "My parasite itches.")
  END()

END_EVENT_SCRIPT()
