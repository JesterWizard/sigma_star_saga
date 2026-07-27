#include "event_macros.h"

/* Script id 286 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806B7D3. */
EVENT_SCRIPT_REPLACEMENT(0x0806B7D3, scene_06B7D3)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme...",
      "What happened to your face?"
  )
  END()

END_EVENT_SCRIPT()
