#include "event_macros.h"

/* Script id 312 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806DC8D. */
EVENT_SCRIPT_REPLACEMENT(0x0806DC8D, scene_06DC8D)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Go get that genome.")
  END()

END_EVENT_SCRIPT()
