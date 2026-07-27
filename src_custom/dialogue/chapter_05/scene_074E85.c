#include "event_macros.h"

/* Script id 418 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074E85. */
EVENT_SCRIPT_REPLACEMENT(0x08074E85, scene_074E85)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "...",
      "What?",
      "Who's there?"
  )
  END()

END_EVENT_SCRIPT()
