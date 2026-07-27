#include "event_macros.h"

/* Script id 301 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806CB16. */
EVENT_SCRIPT_REPLACEMENT(0x0806CB16, scene_06CB16)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Gasp...Gasp...",
      "Wha...",
      "That's Ek...",
      "Dead!"
  )
  END()

END_EVENT_SCRIPT()
