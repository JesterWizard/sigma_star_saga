#include "event_macros.h"

/* Script id 399 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08073680. */
EVENT_SCRIPT_REPLACEMENT(0x08073680, scene_073680)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Devils! The monsters butchered our planet to destroy another!! ")
  END()

END_EVENT_SCRIPT()
