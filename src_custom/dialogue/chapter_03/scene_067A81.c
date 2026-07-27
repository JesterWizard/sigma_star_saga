#include "event_macros.h"

/* Script id 201 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08067A81. */
EVENT_SCRIPT_REPLACEMENT(0x08067A81, scene_067A81)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Did you just call me?")
  END()

END_EVENT_SCRIPT()
