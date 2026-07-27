#include "event_macros.h"

/* Script id 70 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805D8A8. */
EVENT_SCRIPT_REPLACEMENT(0x0805D8A8, scene_05D8A8)

  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "What's...",
      "What...did",
      "You...",
      "Do...?"
  )
  END()

END_EVENT_SCRIPT()
