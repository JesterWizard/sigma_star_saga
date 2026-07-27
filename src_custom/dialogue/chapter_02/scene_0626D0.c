#include "event_macros.h"

/* Script id 138 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080626D0. */
EVENT_SCRIPT_REPLACEMENT(0x080626D0, scene_0626D0)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Hustle hustle hustle!")
  END()

END_EVENT_SCRIPT()
