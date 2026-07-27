#include "event_macros.h"

/* Script id 361 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0807103C. */
EVENT_SCRIPT_REPLACEMENT(0x0807103C, scene_07103C)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Zelly? No commander by that name here.")
  END()

END_EVENT_SCRIPT()
