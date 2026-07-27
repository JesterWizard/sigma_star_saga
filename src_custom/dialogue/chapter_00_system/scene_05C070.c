#include "event_macros.h"

/* Script id 10 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C070. */
EVENT_SCRIPT_REPLACEMENT(0x0805C070, scene_05C070)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hello.")
  END()

END_EVENT_SCRIPT()
