#include "event_macros.h"

/* Script id 61 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CE0C. */
EVENT_SCRIPT_REPLACEMENT(0x0805CE0C, scene_05CE0C)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "I see you have a MAP! Press SELECT to take a look.")
  END()

END_EVENT_SCRIPT()
