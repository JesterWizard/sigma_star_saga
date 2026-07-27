#include "event_macros.h"

/* Script id 266 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806A525. */
EVENT_SCRIPT_REPLACEMENT(0x0806A525, scene_06A525)

  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "But Recker...")
  END()

END_EVENT_SCRIPT()
