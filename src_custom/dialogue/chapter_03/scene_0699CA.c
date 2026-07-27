#include "event_macros.h"

/* Script id 251 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080699CA. */
EVENT_SCRIPT_REPLACEMENT(0x080699CA, scene_0699CA)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "We can't leave the ship till the bombing's done. Man I'm getting bored.")
  END()

END_EVENT_SCRIPT()
