#include "event_macros.h"

/* Script id 462 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08077C82. */
EVENT_SCRIPT_REPLACEMENT(0x08077C82, scene_077C82)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme!",
      "Psyme! Wake up!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "...moan...",
      "..."
  )
  END()

END_EVENT_SCRIPT()
