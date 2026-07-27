#include "event_macros.h"

/* Script id 208 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08067FBC. */
EVENT_SCRIPT_REPLACEMENT(0x08067FBC, scene_067FBC)

  TALK(SPEAKER_UNKNOWN, SIDE_RIGHT, EXPR_NEUTRAL,
      "Help! Help me! They're everywhere!")
  END()

END_EVENT_SCRIPT()
