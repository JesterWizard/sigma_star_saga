#include "event_macros.h"

/* Script id 234 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08069034. */
EVENT_SCRIPT_REPLACEMENT(0x08069034, scene_069034)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Earthgirl and I've been blasting out a path that should point us in the right direction. One more should do the trick.")
  END()

END_EVENT_SCRIPT()
