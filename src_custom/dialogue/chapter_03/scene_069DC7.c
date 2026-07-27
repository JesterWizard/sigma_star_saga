#include "event_macros.h"

/* Script id 257 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08069DC7. */
EVENT_SCRIPT_REPLACEMENT(0x08069DC7, scene_069DC7)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme!")
  END()

END_EVENT_SCRIPT()
