#include "event_macros.h"

/* Script id 85 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805EC18. */
EVENT_SCRIPT_REPLACEMENT(0x0805EC18, scene_05EC18)

  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "If it isn't the new guy.",
      "Emergency call came in. Come back in a few minutes, will you?"
  )
  END()

END_EVENT_SCRIPT()
