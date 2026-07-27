#include "event_macros.h"

/* Script id 223 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068DD8. */
EVENT_SCRIPT_REPLACEMENT(0x08068DD8, scene_068DD8)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "All right, 8 more to go.")
  END()

END_EVENT_SCRIPT()
