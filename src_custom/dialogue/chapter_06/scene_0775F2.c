#include "event_macros.h"

/* Script id 459 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080775F2. */
EVENT_SCRIPT_REPLACEMENT(0x080775F2, scene_0775F2)

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
