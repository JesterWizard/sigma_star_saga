#include "event_macros.h"

/* Script id 54 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CAC0. */
EVENT_SCRIPT_REPLACEMENT(0x0805CAC0, scene_05CAC0)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Score one for Earth!")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Yeah, guess we did OK.")
  END()

END_EVENT_SCRIPT()
