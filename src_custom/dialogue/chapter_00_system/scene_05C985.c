#include "event_macros.h"

/* Script id 49 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C985. */
EVENT_SCRIPT_REPLACEMENT(0x0805C985, scene_05C985)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Great time to be a soldier!")
  END()

END_EVENT_SCRIPT()
