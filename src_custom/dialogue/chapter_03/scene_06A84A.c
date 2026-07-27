#include "event_macros.h"

/* Script id 270 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806A84A. */
EVENT_SCRIPT_REPLACEMENT(0x0806A84A, scene_06A84A)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Dead. Then High Command is really cleaning things up.",
      "Don't think I'll go without a fight."
  )
  END()

END_EVENT_SCRIPT()
