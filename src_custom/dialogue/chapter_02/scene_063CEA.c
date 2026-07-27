#include "event_macros.h"

/* Script id 158 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08063CEA. */
EVENT_SCRIPT_REPLACEMENT(0x08063CEA, scene_063CEA)

  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Not now Earthman. I'm calibrating.")
  END()

END_EVENT_SCRIPT()
