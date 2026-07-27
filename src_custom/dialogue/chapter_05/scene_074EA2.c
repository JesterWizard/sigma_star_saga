#include "event_macros.h"

/* Script id 419 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074EA2. */
EVENT_SCRIPT_REPLACEMENT(0x08074EA2, scene_074EA2)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "What is this! Get out of my office!")
  END()

END_EVENT_SCRIPT()
