#include "event_macros.h"

/* Script id 28 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C451. */
EVENT_SCRIPT_REPLACEMENT(0x0805C451, scene_05C451)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Do they have xyzentles on Earth? I couldn't imagine life without oneof those.")
  END()

END_EVENT_SCRIPT()
