#include "event_macros.h"

/* Script id 188 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08066186. */
EVENT_SCRIPT_REPLACEMENT(0x08066186, scene_066186)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "They found her access key and everything. And on top of all that, she still didn't clean the Comm Room! It won't clean itself!")
  END()

END_EVENT_SCRIPT()
