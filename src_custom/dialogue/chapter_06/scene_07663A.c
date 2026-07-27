#include "event_macros.h"

/* Script id 436 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0807663A. */
EVENT_SCRIPT_REPLACEMENT(0x0807663A, scene_07663A)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's that noise?")
  END()

END_EVENT_SCRIPT()
