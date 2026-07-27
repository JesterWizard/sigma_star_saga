#include "event_macros.h"

/* Script id 16 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C1BD. */
EVENT_SCRIPT_REPLACEMENT(0x0805C1BD, scene_05C1BD)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Nice shooting out there. I saw you on the holoframe.")
  END()

END_EVENT_SCRIPT()
