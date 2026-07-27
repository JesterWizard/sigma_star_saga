#include "event_macros.h"

/* Script id 238 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08069203. */
EVENT_SCRIPT_REPLACEMENT(0x08069203, scene_069203)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Go! We'll be right behind you!")
  END()

END_EVENT_SCRIPT()
