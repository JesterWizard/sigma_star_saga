#include "event_macros.h"

/* Script id 458 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080775CC. */
EVENT_SCRIPT_REPLACEMENT(0x080775CC, scene_0775CC)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme...",
      "I'll never forget you!"
  )
  END()

END_EVENT_SCRIPT()
