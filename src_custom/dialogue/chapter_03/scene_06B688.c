#include "event_macros.h"

/* Script id 282 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806B688. */
EVENT_SCRIPT_REPLACEMENT(0x0806B688, scene_06B688)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This must be the BLACK BOX. I'd better get it back to Commander Nomak. Maybe I can use it to free Scarlet from those idiots.")
  END()

END_EVENT_SCRIPT()
