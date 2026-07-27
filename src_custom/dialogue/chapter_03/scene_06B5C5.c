#include "event_macros.h"

/* Script id 279 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806B5C5. */
EVENT_SCRIPT_REPLACEMENT(0x0806B5C5, scene_06B5C5)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "We have one of the finest military forces in the galaxy.")
  END()

END_EVENT_SCRIPT()
