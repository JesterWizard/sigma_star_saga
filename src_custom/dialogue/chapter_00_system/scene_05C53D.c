#include "event_macros.h"

/* Script id 31 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C53D. */
EVENT_SCRIPT_REPLACEMENT(0x0805C53D, scene_05C53D)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "This base can hold 10,000 Krill. It doesn't right now, but it could if it had to.")
  END()

END_EVENT_SCRIPT()
