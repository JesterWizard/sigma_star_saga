#include "event_macros.h"

/* Script id 99 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805FBC6. */
EVENT_SCRIPT_REPLACEMENT(0x0805FBC6, scene_05FBC6)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Are you finished?")
  END()

END_EVENT_SCRIPT()
