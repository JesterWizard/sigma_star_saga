#include "event_macros.h"

/* Script id 314 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806DCCB. */
EVENT_SCRIPT_REPLACEMENT(0x0806DCCB, scene_06DCCB)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Ok Scarlet. How do we do this?")
  END()

END_EVENT_SCRIPT()
