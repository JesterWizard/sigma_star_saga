#include "event_macros.h"

/* Script id 335 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E73F. */
EVENT_SCRIPT_REPLACEMENT(0x0806E73F, scene_06E73F)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Oh crumb.")
  END()

END_EVENT_SCRIPT()
