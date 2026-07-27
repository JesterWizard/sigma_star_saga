#include "event_macros.h"

/* Script id 107 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806057F. */
EVENT_SCRIPT_REPLACEMENT(0x0806057F, scene_06057F)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Whooops......Here I go!")
  END()

END_EVENT_SCRIPT()
