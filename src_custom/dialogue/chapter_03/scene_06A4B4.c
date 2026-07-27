#include "event_macros.h"

/* Script id 265 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806A4B4. */
EVENT_SCRIPT_REPLACEMENT(0x0806A4B4, scene_06A4B4)

  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "I gave him some busy work on the Forest Planet to buy a little time. You'll find him somewhere down there.")
  END()

END_EVENT_SCRIPT()
