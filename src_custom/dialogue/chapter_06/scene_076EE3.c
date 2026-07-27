#include "event_macros.h"

/* Script id 446 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08076EE3. */
EVENT_SCRIPT_REPLACEMENT(0x08076EE3, scene_076EE3)

  TALK(SPEAKER_OVERLORD, SIDE_RIGHT, EXPR_NEUTRAL,
      "I...")
  END()

END_EVENT_SCRIPT()
