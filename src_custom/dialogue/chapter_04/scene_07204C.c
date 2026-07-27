#include "event_macros.h"

/* Script id 379 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0807204C. */
EVENT_SCRIPT_REPLACEMENT(0x0807204C, scene_07204C)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's your problem now.")
  END()

END_EVENT_SCRIPT()
