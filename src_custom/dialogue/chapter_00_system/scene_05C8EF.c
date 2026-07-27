#include "event_macros.h"

/* Script id 47 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C8EF. */
EVENT_SCRIPT_REPLACEMENT(0x0805C8EF, scene_05C8EF)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Don't let RoK know I'm here! I'm not done with the project tool yet.")
  END()

END_EVENT_SCRIPT()
