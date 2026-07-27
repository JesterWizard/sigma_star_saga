#include "event_macros.h"

/* Script id 211 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806832E. */
EVENT_SCRIPT_REPLACEMENT(0x0806832E, scene_06832E)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Oh, I see...",
      "Good catch Recker!"
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Who's this?")
  END()

END_EVENT_SCRIPT()
