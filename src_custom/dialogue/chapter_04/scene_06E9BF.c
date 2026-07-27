#include "event_macros.h"

/* Script id 340 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E9BF. */
EVENT_SCRIPT_REPLACEMENT(0x0806E9BF, scene_06E9BF)

  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Uh, good luck pal!")
  END()

END_EVENT_SCRIPT()
