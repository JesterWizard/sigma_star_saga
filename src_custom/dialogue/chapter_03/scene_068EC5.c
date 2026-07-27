#include "event_macros.h"

/* Script id 231 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068EC5. */
EVENT_SCRIPT_REPLACEMENT(0x08068EC5, scene_068EC5)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "That's 10! Gotta get back to the girls before they kill each other.")
  END()

END_EVENT_SCRIPT()
