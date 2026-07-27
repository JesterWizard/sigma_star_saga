#include "event_macros.h"

/* Script id 422 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074F0C. */
EVENT_SCRIPT_REPLACEMENT(0x08074F0C, scene_074F0C)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "EARRRRRRGH...!!!")
  END()

END_EVENT_SCRIPT()
