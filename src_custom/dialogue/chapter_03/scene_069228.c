#include "event_macros.h"

/* Script id 239 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08069228. */
EVENT_SCRIPT_REPLACEMENT(0x08069228, scene_069228)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We don't have much time!")
  END()

END_EVENT_SCRIPT()
