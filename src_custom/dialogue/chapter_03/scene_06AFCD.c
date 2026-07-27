#include "event_macros.h"

/* Script id 274 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806AFCD. */
EVENT_SCRIPT_REPLACEMENT(0x0806AFCD, scene_06AFCD)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "It is.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'm leaving. Your security is air tight now.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "If it isn't, we'll both know where to look.")
  END()

END_EVENT_SCRIPT()
