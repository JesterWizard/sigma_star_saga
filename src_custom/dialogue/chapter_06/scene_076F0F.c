#include "event_macros.h"

/* Script id 449 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08076F0F. */
EVENT_SCRIPT_REPLACEMENT(0x08076F0F, scene_076F0F)

  TALK(SPEAKER_OVERLORD, SIDE_RIGHT, EXPR_NEUTRAL,
      "IN POWER!")
  END()

END_EVENT_SCRIPT()
