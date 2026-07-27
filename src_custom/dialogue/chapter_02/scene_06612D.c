#include "event_macros.h"

/* Script id 186 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806612D. */
EVENT_SCRIPT_REPLACEMENT(0x0806612D, scene_06612D)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "I can't believe she was a double agent!")
  END()

END_EVENT_SCRIPT()
