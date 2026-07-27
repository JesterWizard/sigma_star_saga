#include "event_macros.h"

/* Script id 389 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08072A88. */
EVENT_SCRIPT_REPLACEMENT(0x08072A88, scene_072A88)

  TALK(SPEAKER_FOLLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Specter begs to see Folly. Have to go.")
  END()

END_EVENT_SCRIPT()
