#include "event_macros.h"

/* Script id 447 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08076EEE. */
EVENT_SCRIPT_REPLACEMENT(0x08076EEE, scene_076EEE)

  TALK(SPEAKER_OVERLORD, SIDE_RIGHT, EXPR_NEUTRAL,
      "Will be...")
  END()

END_EVENT_SCRIPT()
