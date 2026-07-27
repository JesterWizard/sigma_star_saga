#include "event_macros.h"

/* Script id 345 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806EF78. */
EVENT_SCRIPT_REPLACEMENT(0x0806EF78, scene_06EF78)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "PSYME!!",
      "Come back!"
  )
  END()

END_EVENT_SCRIPT()
