#include "event_macros.h"

/* Script id 307 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806D0FD. */
EVENT_SCRIPT_REPLACEMENT(0x0806D0FD, scene_06D0FD)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "No sudden moves. Go directly to the Professor.")
  END()

END_EVENT_SCRIPT()
