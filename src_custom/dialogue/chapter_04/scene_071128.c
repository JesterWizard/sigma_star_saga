#include "event_macros.h"

/* Script id 365 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08071128. */
EVENT_SCRIPT_REPLACEMENT(0x08071128, scene_071128)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Sorry, can't help you.")
  END()

END_EVENT_SCRIPT()
