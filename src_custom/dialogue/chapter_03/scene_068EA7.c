#include "event_macros.h"

/* Script id 230 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068EA7. */
EVENT_SCRIPT_REPLACEMENT(0x08068EA7, scene_068EA7)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes! Only 1 more to go!")
  END()

END_EVENT_SCRIPT()
