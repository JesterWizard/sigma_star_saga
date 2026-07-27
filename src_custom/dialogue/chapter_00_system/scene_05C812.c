#include "event_macros.h"

/* Script id 43 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C812. */
EVENT_SCRIPT_REPLACEMENT(0x0805C812, scene_05C812)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Nice parasite. Mine's a little plain.")
  END()

END_EVENT_SCRIPT()
