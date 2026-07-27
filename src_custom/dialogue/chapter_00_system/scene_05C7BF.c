#include "event_macros.h"

/* Script id 42 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C7BF. */
EVENT_SCRIPT_REPLACEMENT(0x0805C7BF, scene_05C7BF)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Do you know how old you are in Krill years? Not very old, I'll say that much.")
  END()

END_EVENT_SCRIPT()
