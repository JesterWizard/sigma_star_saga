#include "event_macros.h"

/* Script id 194 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08067635. */
EVENT_SCRIPT_REPLACEMENT(0x08067635, scene_067635)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker, wake up!")
  END()

END_EVENT_SCRIPT()
