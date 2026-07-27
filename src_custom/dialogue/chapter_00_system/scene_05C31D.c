#include "event_macros.h"

/* Script id 21 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C31D. */
EVENT_SCRIPT_REPLACEMENT(0x0805C31D, scene_05C31D)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "My name is Mondo. I am the mizzan.")
  END()

END_EVENT_SCRIPT()
