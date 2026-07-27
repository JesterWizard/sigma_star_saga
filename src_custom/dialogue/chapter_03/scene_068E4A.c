#include "event_macros.h"

/* Script id 227 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068E4A. */
EVENT_SCRIPT_REPLACEMENT(0x08068E4A, scene_068E4A)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hmmm...4 more to go.")
  END()

END_EVENT_SCRIPT()
