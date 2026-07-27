#include "event_macros.h"

/* Script id 261 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806A126. */
EVENT_SCRIPT_REPLACEMENT(0x0806A126, scene_06A126)

  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Blune.")
  END()

END_EVENT_SCRIPT()
