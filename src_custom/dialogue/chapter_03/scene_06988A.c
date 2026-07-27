#include "event_macros.h"

/* Script id 248 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806988A. */
EVENT_SCRIPT_REPLACEMENT(0x0806988A, scene_06988A)

  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "So you kids clear out. I want to get better acquainted with my new pet here. Get the door on your way out.")
  END()

END_EVENT_SCRIPT()
