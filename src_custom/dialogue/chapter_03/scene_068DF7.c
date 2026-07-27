#include "event_macros.h"

/* Script id 224 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068DF7. */
EVENT_SCRIPT_REPLACEMENT(0x08068DF7, scene_068DF7)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Fantastic! 7 more to go.")
  END()

END_EVENT_SCRIPT()
