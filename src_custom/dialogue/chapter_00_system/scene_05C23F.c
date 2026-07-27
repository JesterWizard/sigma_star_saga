#include "event_macros.h"

/* Script id 18 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C23F. */
EVENT_SCRIPT_REPLACEMENT(0x0805C23F, scene_05C23F)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "This is my post now. I used to be in Hall 3.")
  END()

END_EVENT_SCRIPT()
