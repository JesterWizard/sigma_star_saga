#include "event_macros.h"

/* Script id 45 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C874. */
EVENT_SCRIPT_REPLACEMENT(0x0805C874, scene_05C874)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Nice to have you aboard. We don't see many new faces.")
  END()

END_EVENT_SCRIPT()
