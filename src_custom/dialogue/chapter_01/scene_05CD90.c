#include "event_macros.h"

/* Script id 59 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CD90. */
EVENT_SCRIPT_REPLACEMENT(0x0805CD90, scene_05CD90)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "The commander's waiting.\nGo on in.")
  END()

END_EVENT_SCRIPT()
