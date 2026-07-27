#include "event_macros.h"

/* Script id 155 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08063B64. */
EVENT_SCRIPT_REPLACEMENT(0x08063B64, scene_063B64)

  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Use the NAV COM to land.")
  END()

END_EVENT_SCRIPT()
