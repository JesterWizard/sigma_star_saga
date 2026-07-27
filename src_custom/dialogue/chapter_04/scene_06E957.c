#include "event_macros.h"

/* Script id 339 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E957. */
EVENT_SCRIPT_REPLACEMENT(0x0806E957, scene_06E957)

  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Here she is. This should be a private moment between the two of you, so we will excuse ourselves.")
  END()

END_EVENT_SCRIPT()
