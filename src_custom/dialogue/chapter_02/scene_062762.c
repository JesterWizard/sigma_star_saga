#include "event_macros.h"

/* Script id 141 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08062762. */
EVENT_SCRIPT_REPLACEMENT(0x08062762, scene_062762)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Not now! See Commander Zelly!")
  END()

END_EVENT_SCRIPT()
