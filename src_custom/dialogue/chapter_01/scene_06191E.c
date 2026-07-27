#include "event_macros.h"

/* Script id 126 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806191E. */
EVENT_SCRIPT_REPLACEMENT(0x0806191E, scene_06191E)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Shaking planets eh? That's weird. I thought we'd be stationed here for something more important.")
  END()

END_EVENT_SCRIPT()
