#include "event_macros.h"

/* Script id 174 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08065381. */
EVENT_SCRIPT_REPLACEMENT(0x08065381, scene_065381)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Hey. Thanks for saving Blune. I owe you one.")
  END()

END_EVENT_SCRIPT()
