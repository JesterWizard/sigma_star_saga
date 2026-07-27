#include "event_macros.h"

/* Script id 385 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080724B8. */
EVENT_SCRIPT_REPLACEMENT(0x080724B8, scene_0724B8)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Congratulations on your promotion sir! I did not realize you're from off world! Quite an achievement!")
  END()

END_EVENT_SCRIPT()
