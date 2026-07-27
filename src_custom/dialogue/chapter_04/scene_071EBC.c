#include "event_macros.h"

/* Script id 376 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08071EBC. */
EVENT_SCRIPT_REPLACEMENT(0x08071EBC, scene_071EBC)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Wait...Something's happening ahead.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Get out of there!")
  END()

END_EVENT_SCRIPT()
