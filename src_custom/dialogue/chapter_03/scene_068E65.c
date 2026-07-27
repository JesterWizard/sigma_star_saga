#include "event_macros.h"

/* Script id 228 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068E65. */
EVENT_SCRIPT_REPLACEMENT(0x08068E65, scene_068E65)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Good. 3 more to go.")
  END()

END_EVENT_SCRIPT()
