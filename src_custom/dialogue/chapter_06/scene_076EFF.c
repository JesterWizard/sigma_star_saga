#include "event_macros.h"

/* Script id 448 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08076EFF. */
EVENT_SCRIPT_REPLACEMENT(0x08076EFF, scene_076EFF)

  TALK(SPEAKER_OVERLORD, SIDE_RIGHT, EXPR_NEUTRAL,
      "UNEQUALED")
  END()

END_EVENT_SCRIPT()
