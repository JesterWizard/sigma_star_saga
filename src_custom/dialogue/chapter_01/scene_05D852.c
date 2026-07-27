#include "event_macros.h"

/* Script id 68 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805D852. */
EVENT_SCRIPT_REPLACEMENT(0x0805D852, scene_05D852)

  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "To Sigma Team.")
  END()

END_EVENT_SCRIPT()
