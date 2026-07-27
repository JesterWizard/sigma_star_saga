#include "event_macros.h"

/* Script id 136 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08062695. */
EVENT_SCRIPT_REPLACEMENT(0x08062695, scene_062695)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "No time to talk, Earthman!")
  END()

END_EVENT_SCRIPT()
