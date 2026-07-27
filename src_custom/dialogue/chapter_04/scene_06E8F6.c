#include "event_macros.h"

/* Script id 338 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E8F6. */
EVENT_SCRIPT_REPLACEMENT(0x0806E8F6, scene_06E8F6)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This pond looks good enough. I'm signaling! I'm signaling!",
      "Oh man, I'm going to be a dad!!"
  )
  END()

END_EVENT_SCRIPT()
