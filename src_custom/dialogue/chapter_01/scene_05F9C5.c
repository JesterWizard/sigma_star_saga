#include "event_macros.h"

/* Script id 93 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805F9C5. */
EVENT_SCRIPT_REPLACEMENT(0x0805F9C5, scene_05F9C5)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Wha...?")
  END()

END_EVENT_SCRIPT()
