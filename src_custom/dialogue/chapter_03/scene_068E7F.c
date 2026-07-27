#include "event_macros.h"

/* Script id 229 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068E7F. */
EVENT_SCRIPT_REPLACEMENT(0x08068E7F, scene_068E7F)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Gettin' close! Only 2 more to go.")
  END()

END_EVENT_SCRIPT()
