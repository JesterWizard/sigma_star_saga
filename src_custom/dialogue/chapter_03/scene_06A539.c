#include "event_macros.h"

/* Script id 267 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806A539. */
EVENT_SCRIPT_REPLACEMENT(0x0806A539, scene_06A539)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yeah?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't trust Commander Zelly for a second.")
  END()

END_EVENT_SCRIPT()
