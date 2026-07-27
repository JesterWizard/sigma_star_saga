#include "event_macros.h"

/* Script id 132 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08061FF8. */
EVENT_SCRIPT_REPLACEMENT(0x08061FF8, scene_061FF8)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I got the news. We'll depart for Starbase 2 right after I complete my report.")
  END()

END_EVENT_SCRIPT()
