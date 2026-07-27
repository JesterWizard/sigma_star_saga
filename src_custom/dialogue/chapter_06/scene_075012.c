#include "event_macros.h"

/* Script id 425 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08075012. */
EVENT_SCRIPT_REPLACEMENT(0x08075012, scene_075012)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
      "Huh?")
  END()

END_EVENT_SCRIPT()
