#include "event_macros.h"

/* Script id 137 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080626B6. */
EVENT_SCRIPT_REPLACEMENT(0x080626B6, scene_0626B6)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Too busy, Earthman!")
  END()

END_EVENT_SCRIPT()
