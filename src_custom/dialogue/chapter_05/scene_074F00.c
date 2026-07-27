#include "event_macros.h"

/* Script id 421 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074F00. */
EVENT_SCRIPT_REPLACEMENT(0x08074F00, scene_074F00)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "HRGH!")
  END()

END_EVENT_SCRIPT()
