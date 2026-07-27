#include "event_macros.h"

/* Script id 200 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08067A48. */
EVENT_SCRIPT_REPLACEMENT(0x08067A48, scene_067A48)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't break my concentration. Nope. Still nothing.")
  END()

END_EVENT_SCRIPT()
