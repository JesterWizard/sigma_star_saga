#include "event_macros.h"

/* Script id 151 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08063305. */
EVENT_SCRIPT_REPLACEMENT(0x08063305, scene_063305)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Make sure you clean around the touch panel. Dust builds up there and drives Commander Zelly nuts.")
  END()

END_EVENT_SCRIPT()
